#ifndef UTILITIES_H
#define UTILITIES_H
#include "mySDL.h"

// Screen dimensions
// 320x480 with 16x16 textures => 20x30 => 600 objects

//Texture dimension
const int TEXTURE_WIDTH = 16;
const int TEXTURE_HEIGHT = 16;

const int TILE_SIZE = 16; // this isn't ideal which is why I will avoid using it as much as possible
const float GRAVITY = 980000.0f; // Pixels per second per second, really big values because of the difference in framerate between laptop and board
const float JUMP_STRENGTH = -4000.0f; // Negative is up, pixels per second
const float MOVE_SPEED = 22500.0f; // Pixels per second for horizontal movement
const float GOOMBA_MOVE_SPEED = 100.0f;
const float MAX_FALL_SPEED = 980000.0f;                                 
enum WorldEnum{
    WORLD_1,
    WORLD_2,
    WORLD_3,
    WORLD_4
};     
         
enum LevelEnum{
    LEVEL_1, 
    LEVEL_2,
    LEVEL_3,
    LEVEL_4
};         

int toInt(std::string id){
    int value = 0;
    for(int i = 0; i < id.size(); i++)
        value *= 10 + static_cast<int>(id[i] - '0');
    return value;
}

const char* worldEnumToChar(WorldEnum world) {
    switch (world) {
        case WorldEnum::WORLD_1: return "1";
        case WorldEnum::WORLD_2: return "2";
        case WorldEnum::WORLD_3: return "3";
        case WorldEnum::WORLD_4: return "4";    
        default: return "INVALID_WORLD";
    }
}

const char* levelEnumToChar(LevelEnum level) {
    switch (level) {
        case LevelEnum::LEVEL_1: return "1";
        case LevelEnum::LEVEL_2: return "2";
        case LevelEnum::LEVEL_3: return "3";
        case LevelEnum::LEVEL_4: return "4";
        default: return "INVALID_LEVEL";
    }
}

#endif
