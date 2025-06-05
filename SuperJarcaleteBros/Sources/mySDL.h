#ifndef MYSDL_H
#define MYSDL_H

#include <sys/mman.h>  // for mmap
#include "mzapo_template/mzapo_parlcd.c"
#include "mzapo_template/mzapo_regs.h"
#include "mzapo_template/font_types.h"

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
                    image[dstY * SCREEN_WIDTH + dstX] = sprite[dstY * width + dstX];
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
            
    void initialScreen(){
        for(int i = 0; i < SCREEN_WIDTH; i++)
            for(int j = 0; j < SCREEN_HEIGTH; j++)
                if((j > 120 && j < 240) && (i > 120 && i < 240))
                    image[i * width + j] = 0xFFFF;
                else image[i * width + j] = 0x0000;
                    
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

   void draw_char(int x, int y, char c, font_descriptor_t *font, uint16_t color) {
        if (c < font->firstchar || c >= font->firstchar + font->size)
            c = font->defaultchar;

        int index = c - font->firstchar;
        const font_bits_t *bitmap = font->bits + font->offset[index];
        int width = font->width ? font->width[index] : font->maxwidth;

        for (int row = 0; row < font->height; row++) {
            font_bits_t bits = bitmap[row]; 
            for (int col = 0; col < width; col++) {
                if (bits & (1 << (15 - col))) {
                    int px = x + col;
                    int py = y + row;
                    image[py * SCREEN_WIDTH + px] = color;
                }
            }
    }
}
                      
    void draw_text(int x, int y, const char *text, font_descriptor_t *font, uint16_t color) {
    int cursor_x = x;
    while (*text) {
        draw_char( cursor_x, y, *text, font, color);
        int char_index = *text - font->firstchar;
        int char_width = font->width ? font->width[char_index] : font->maxwidth;
        cursor_x += char_width + 1; 
        text++;
    }
}

    
    void displayStartScreen(){
        initialScreen();
        render();
    
        const char* message = "Press red button to start";
        uint16_t color_white = 0xFFFF; //white because yes

        int total_text_width = 0;
        int inter_char_spacing = 1;
        const char* temp_ptr = message;
        while (*temp_ptr) {
            char c = *temp_ptr;
            int char_font_index;
            int char_pixel_width;

            if (c < font_rom8x16.firstchar || c >= font_rom8x16.firstchar + font_rom8x16.size) {
                char_font_index = font_rom8x16.defaultchar - font_rom8x16.firstchar;
                if (char_font_index < 0 || char_font_index >= font_rom8x16.size) char_font_index = 0;
            } else char_font_index = c - font_rom8x16.firstchar;

        char_pixel_width = font_rom8x16.width ? font_rom8x16.width[char_font_index] : font_rom8x16.maxwidth;
        
        total_text_width += char_pixel_width;
        if (*(temp_ptr + 1))
            total_text_width += inter_char_spacing;
        temp_ptr++;
        }

        int start_x = 121;
        int start_y = 121;

        if (start_x < 0) start_x = 0;
        if (start_y < 0) start_y = 0;

        draw_text(start_x, start_y, message, &font_rom8x16, color_white);
    
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
