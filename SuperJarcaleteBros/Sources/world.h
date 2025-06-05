#ifndef WORLD_H
#define WORLD_H

#include "utilities.h"
#include "tileAssetManager.h"
#include "camera.h"

class world{
public:
    int* loadTileMap(WorldEnum world, LevelEnum level){
        //delete tileMap;
        char idDataPath[256] = "Assets/Worlds/World1/1-1idData.txt";
        char mapLayoutPath[256] = "Assets/Worlds/World1/1-1.txt"; 
        //snprintf(idDataPath, sizeof(idDataPath), "Assets/Worlds/World%s/%s-%sidData.txt", worldEnumToCStr(world), worldEnumToCStr(world), levelEnumToCStr(level));

        //snprintf(mapLayoutPath, sizeof(mapLayoutPath), "Assets/Worlds/World%s/%s-%s.txt", worldEnumToCStr(world), worldEnumToCStr(world), levelEnumToCStr(level));
        /*
        char* filePathId = (char*)malloc(sizeof(char) * 34); // 34 is the size of the file path, cool I know;
        filePathId[0] = '\0';
        auto basePath = "Assets/Worlds/World";                  //auto 'cause lazy, sorry                                   
        strcpy(filePathId, basePath);
        strcat(filePathId, worldEnumToChar(world));
        strcat(filePathId, "/");
        strcat(filePathId, worldEnumToChar(world));
        strcat(filePathId, "-");
        strcat(filePathId, levelEnumToChar(level));
        char* filePath = (char*)malloc(sizeof(char) * 28); //28 here
        strcpy(filePath, filePathId);
        strcat(filePathId, "idData.txt");
        strcat(filePath, ".txt");
        */
        std::ifstream IdDataFile(filePathId);
        int* idList;
        IdDataFile >> totalNrOfIds;
        idList = (int*)malloc(totalNrOfIds * sizeof(int));
        for(int i = 0; i < totalNrOfIds; i++)
            IdDataFile >> idList[i];
        
        IdDataFile.close();
        
        std::ifstream tileMapFile(filePath);
        
        tileMapFile >> tileMapWidth >> tileMapHeigth;
        tileMapSize = tileMapWidth * tileMapHeigth;
        tileMap = (int*)malloc(sizeof(int) * tileMapSize);
        for(int i = 0; i < tileMapSize; i++)
            tileMapFile >> tileMap[i];
        tileMapFile.close();
        
        worldWidthPixels = tileMapWidth * TEXTURE_WIDTH;
        //if(worldWidthPixels < SCREEN_WIDTH)
          //  worldWidthPixels = SCREEN_WIDTH;
        
        worldHeigthPixels = tileMapHeigth * TEXTURE_HEIGHT;
        //if(worldHeigthPixels < SCREEN_HEIGHT)
          //  worldHeigthPixels = SCREEN_HEIGHT;

       // free(filePathId);
        //free(filePath);
        return idList;
    }
    
    uint16_t* getTexture(uint16_t *spriteSheet, SDL_Rect srcRect){
        int size = 16 * 16, x = 0, y = 0;
        uint16_t* sprite = new uint16_t[size];
        for(int i = 0; i < 16; i++)
            for(int j = 0; j < 16; j++){
                x = srcRect.x + i;
                y = srcRect.y + j;
                sprite[i * 16 + j] = spriteSheet[x * 16 + y];
            }
        return sprite;
    }
    
    void render(SDL_Renderer* renderer, camera* Camera, tileAssetManager* assets) {
        struct timeval start, end;
        gettimeofday(&start, NULL);


        int startCol = std::max(0, static_cast<int>(std::floor(Camera->getX() / TEXTURE_WIDTH)));
        int endCol = std::min(tileMapWidth, static_cast<int>(std::ceil((Camera->getX() + SCREEN_WIDTH) / TEXTURE_WIDTH)));
        int startRow = std::max(0, static_cast<int>(std::floor(Camera->getY() / TEXTURE_HEIGHT)));
        int endRow = std::min(tileMapHeigth, static_cast<int>(std::ceil((Camera->getY() + SCREEN_HEIGHT) / TEXTURE_HEIGHT)));

        uint16_t* spriteSheet = assets->getSpriteSheet();
        for (int row = startRow; row < endRow; row++) {
            for (int col = startCol; col < endCol; col++) {
                int tileID = tileMap[row * tileMapWidth + col];
                if (tileID == 9) continue; // Skip the sky, it's done at the very beggining in class Game

                tileTypeProperties* props = assets->getTileProperties(tileID);
                SDL_Rect srcRect = props->positionOnSpriteSheet;
                uint16_t* texture = getTexture(spriteSheet, srcRect);
                SDL_Rect destRect;
                destRect.x = static_cast<int>((col * TEXTURE_WIDTH) - Camera->getX());
                destRect.y = static_cast<int>((row * TEXTURE_HEIGHT) - Camera->getY()); 
                destRect.w = TEXTURE_WIDTH;
                destRect.h = TEXTURE_HEIGHT;

                renderer->addToMemory(destRect, texture);
            }
        }   
        
        gettimeofday(&end, NULL);
        long ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000;
        printf("Time: %ld ms\n", ms);    
    }       
    
  
    int getTotalNrOfIds(){
        return totalNrOfIds;
    } 
    
    float getPixelWidth(){
        return worldWidthPixels;
    }
    
    float getPixelHeigth(){
        return worldHeigthPixels;
    }
    
    void cleanup(){
        if(tileMap)
            delete tileMap;
    }
    
    int getTileIdAT(int col, int row){
        int tileId = row * tileMapWidth + col;
        if(tileId > tileMapSize)
            return 0;
        return tileMap[tileId];
    }
    
    int getTileSize(){
        return tileSize;
    }
    
private:
    int* tileMap; //flattened 2D, too much CUDA what can I do?
    int tileMapWidth = 0, tileMapHeigth = 0, tileMapSize = 0;
    int totalNrOfIds = 0;
    //int totalNrOfObjects = 0;
    float worldWidthPixels = 0, worldHeigthPixels = 0;
    int tileSize = 16;
//totalNrOfObjects is unused, it could be used to only keep in memory actual objects, skipping the sky which utilises too much memory. For now it's not implemented because of convenience
//but if the performance is too bad because the RAM is somehow overutilised, it is one good idea.
};
#endif

