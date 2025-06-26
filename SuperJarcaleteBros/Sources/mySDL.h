#ifndef MYSDL_H
#define MYSDL_H

#include <sys/mman.h>  // for mmap
#include "mzapo_template/mzapo_parlcd.c"
#include "mzapo_template/mzapo_regs.h"
#include "mzapo_template/font_types.h"
#include "mzapo_template/font_rom8x16.c"

#include "mzapo_template/mzapo_parlcd.h"
#include "mzapo_template/mzapo_phys.h"
#include "mzapo_template/mzapo_regs.h"
#include "mzapo_template/serialize_lock.h"
#include <iostream>
#include <fcntl.h> 
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <cmath>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 320;
struct SDL_Rect{
    float x, y, w, h;
};

volatile uint32_t *spiled_base = nullptr;
class SDL_Renderer{
public:
    void initialise(){
        size = 480 * 320;
        image = (uint16_t*)malloc(size * sizeof(uint16_t));
        int fd;
        void *parlcd_mem_base_aux;

        fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (fd == -1) {
            perror("open /dev/mem");
            return;
        }
        
        parlcd_mem_base_aux = mmap(NULL, PARLCD_REG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PARLCD_REG_BASE_PHYS);
        if (parlcd_mem_base_aux == MAP_FAILED) {
            perror("mmap");
            close(fd);
            return;
        }
    
        spiled_base = (volatile uint32_t *)mmap(NULL, SPILED_REG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, SPILED_REG_BASE_PHYS);
        if (spiled_base == MAP_FAILED) {
            perror("mmap spiled_base");
            spiled_base = nullptr;
            close(fd);
            return;
        }

    close(fd);
    parlcd_mem_base = (unsigned char*)parlcd_mem_base_aux;
    parlcd_hx8357_init(parlcd_mem_base);
    }
                          
    void addToMemory(SDL_Rect rect, uint16_t* sprite){
        int startX = (int)rect.x;
        int startY = (int)rect.y;
        int width = (int)rect.w;
        int height = (int)rect.h;

        for (int j = 0; j < height; j++) 
            for (int i = 0; i < width; i++) {
                int dstX = startX + i;
                int dstY = startY + j;

                if (dstX >= 0 && dstX < SCREEN_WIDTH && dstY >= 0 && dstY < SCREEN_HEIGHT)
                    image[dstY * SCREEN_WIDTH + dstX] = sprite[j * width + i];
            }
    }
                      
    void render(){
        parlcd_write_cmd(parlcd_mem_base, 0x2A);
        parlcd_write_data(parlcd_mem_base, 0); 
        parlcd_write_data(parlcd_mem_base, 0);
        parlcd_write_data(parlcd_mem_base, (SCREEN_WIDTH - 1) >> 8); 
        parlcd_write_data(parlcd_mem_base, (SCREEN_WIDTH - 1) & 0xFF);

        parlcd_write_cmd(parlcd_mem_base, 0x2B);
        parlcd_write_data(parlcd_mem_base, 0);
        parlcd_write_data(parlcd_mem_base, 0);
        parlcd_write_data(parlcd_mem_base, (SCREEN_HEIGHT - 1) >> 8); 
        parlcd_write_data(parlcd_mem_base, (SCREEN_HEIGHT - 1) & 0xFF);

        parlcd_write_cmd(parlcd_mem_base, 0x2C);

        for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
            parlcd_write_data(parlcd_mem_base, image[i]);
    }
        
    void clear(){
        for(int i = 0; i < size; i++)
            image[i] = 0x833C; //sky
    }
            
    void initialScreen() {
    uint16_t blackColor = 0x0000;
    for (int r = 0; r < SCREEN_HEIGHT; ++r) {
        for (int c = 0; c < SCREEN_WIDTH; ++c) {
            image[r * SCREEN_WIDTH + c] = blackColor;
        }
    }
}       
    void cleanup(){
        cleanup_leds();
        free(image);
    }
                      
    void turn_on_leds(uint32_t bitmask) {
        if(spiled_base)
            spiled_base[SPILED_REG_LED_LINE_o / 4] = bitmask;
    }

    void cleanup_leds(){
    if(spiled_base)
        munmap((void *)spiled_base, SPILED_REG_SIZE);
    }


void draw_char(int x, int y, char c, const font_descriptor_t *font, uint16_t color) {
    if(c < font->firstchar || c >= font->firstchar + font->size)
            c = font->firstchar;
    
    int index = c - font->firstchar;

    int width = font->maxwidth; 
    
    const font_bits_t *font_pixel_data_array = (const font_bits_t *)font->bits;
    const font_bits_t *char_bitmap_ptr = font_pixel_data_array + (index * font->height);

    for (int row_idx = 0; row_idx < font->height; row_idx++) { 
        font_bits_t row_pixel_data = char_bitmap_ptr[row_idx]; // Get the uint16_t for the current row

        for (int col_idx = 0; col_idx < width; col_idx++) 
            if (row_pixel_data & (1 << (15 - col_idx))) {
                int screen_x = x + col_idx;
                int screen_y = y + row_idx;

                if (screen_x >= 0 && screen_x < SCREEN_WIDTH && screen_y >= 0 && screen_y < SCREEN_HEIGHT) {
                    image[screen_y * SCREEN_WIDTH + screen_x] = color;
                }
            }
    }
}
                    
    void draw_text(int x, int y, const char *text, font_descriptor_t *font, uint16_t color) {
    int cursor_x = x;
    while (*text) {
        draw_char( cursor_x, y, *text, font, color);
        int char_index = *text - font->firstchar;
        int char_width = font->maxwidth;
        cursor_x += char_width + 1; 
        text++;
    }
}

    
    void displayStartScreen(){
        initialScreen();
        //render();
    
        const char* message = "Press Red Knob To Start";
        uint16_t color_white = 0xFFFF; //white because yes

        int total_text_width = 0;
        int inter_char_spacing = 1;
        const char* temp_ptr = message;
    while (*temp_ptr) {
        total_text_width += font_rom8x16.maxwidth;
        if (*(temp_ptr + 1)) { // If not the last character
            total_text_width += inter_char_spacing;
        }
        temp_ptr++;
    }

        int start_x = 121;
        int start_y = 121;

        if (start_x < 0) start_x = 0;
        if (start_y < 0) start_y = 0;

        draw_text(start_x, start_y, message, &font_rom8x16, color_white);
    
        render();
        
        uint32_t state = spiled_base[SPILED_REG_KNOBS_8BIT_o / 4];
        bool inputRed = (state >> 26) & 1;
        while(inputRed == 0){
            state = spiled_base[SPILED_REG_KNOBS_8BIT_o / 4];
            inputRed = (state >> 26) & 1;
        }
    }
                      
    unsigned char* parlcd_mem_base;
private:
    uint16_t* image;  
    int size = 0;
    
};
#endif
