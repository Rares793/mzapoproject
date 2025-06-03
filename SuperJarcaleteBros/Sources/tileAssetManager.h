#ifndef TILE_ASSET_MANAGER_H
#define TILE_ASSET_MANAGER_H

#include "utilities.h"
//maps the properties and position of the tile in the sprite sheet
struct tileTypeProperties{   
    int isSolid = 0;
    SDL_Rect positionOnSpriteSheet;
    bool isBreakable = 0;
};    

class tileAssetManager{
public:
     ~tileAssetManager(){
         cleanup();
     }
                          
    void initialise(SDL_Renderer* renderer, const char* spriteSheetFilePath, const int* idList, int nrOfIds){
        cleanup();
        
        std::ifstream fin("Assets/Worlds/World1/spriteSheet.txt");
        int numPixels = 16 * 192; //these are the values
        spriteSheet = new uint16_t[numPixels];
        std::string hexValue;
        int i = 0;
        while(fin >> hexValue && i < numPixels)
            spriteSheet[i++] = static_cast<uint16_t>(std::stoul(hexValue, nullptr, 16));
        
        for (int i = 0; i < nrOfIds; i++) { 
            int gameTileID = idList[i];

            tileTypeProperties props;
            
            if (gameTileID != 9) 
                props.isSolid = 1;

            props.positionOnSpriteSheet.x = gameTileID * TEXTURE_WIDTH; 
            props.positionOnSpriteSheet.y = 0;
            props.positionOnSpriteSheet.w = TEXTURE_WIDTH;
            props.positionOnSpriteSheet.h = TEXTURE_HEIGHT;

            tileDefinitions[gameTileID] = props;
        }                                 
    } 
    
    uint16_t* getSpriteSheet() const {
        return spriteSheet;
    }

    tileTypeProperties* getTileProperties(int id){
        return &tileDefinitions[id];
    }
    
    int isTileSolid(int id){
        return tileDefinitions[id].isSolid;
    }
    void cleanup() {
        if (spriteSheet) {
            delete[] spriteSheet;
            spriteSheet = nullptr;
        }
        tileDefinitions.clear();
    }   
                          
private:
    std::map<int, tileTypeProperties> tileDefinitions;
    uint16_t* spriteSheet = nullptr;
};
#endif


