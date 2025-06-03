#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include "world.h"
#include "entity.h"
class collisionBox{
public:
    void checkCollisionWithWorld(float worldWidthPixels, float worldHeigthPixels, float& entityX, float& entityY,
                                 float& entityVX, float& entityVY, SDL_Rect& entityRect, bool& onGround, bool& alive){
        if(entityY + entityRect.h > worldHeigthPixels) {
            entityY = worldHeigthPixels - entityRect.h;
            entityVY = 0;
            onGround = 1;
        }

        if(entityY < 0) { 
            entityY = 0;
            if(entityVY < 0)
                entityVY = 0; 
            alive = false;
            //printf("GG");
        }

        if(entityX < 0) {
            entityX = 0;
            if(entityVX < 0) 
                entityVX = 0;
        }

        if(entityX + entityRect.w > worldWidthPixels) {
            entityX = worldWidthPixels - entityRect.w;
            if(entityVX > 0) 
                entityVX = 0;
        }
    }               
                        
    void checkCollisionY(world* gameWorld, tileAssetManager* assets, float& entityX, float& entityY,
                                 float& entityVX, float& entityVY, SDL_Rect& entityRect, bool& onGround){
        int tileSize = gameWorld->getTileSize();
        float colBoxW = static_cast<float>(entityRect.w);
        float colBoxH = static_cast<float>(entityRect.h);
        int leftTileCol = static_cast<int>(std::floor(entityX / tileSize));
        int rightTileCol = static_cast<int>(std::floor((entityX + colBoxW - 0.1f) / tileSize));
            
        if(entityVY > 0.0f) {
            int bottomTileRow = static_cast<int>(std::floor((entityY + colBoxH) / tileSize));
            for(int currentCol = leftTileCol; currentCol <= rightTileCol; ++currentCol) {
                int tileID = gameWorld->getTileIdAT(currentCol, bottomTileRow);
                if(assets->isTileSolid(tileID)) {
                    entityY = (float)bottomTileRow * TILE_SIZE - colBoxH;
                    entityVY = 0.0f;
                    onGround = true;
                    return;}}} 
            else if(entityVY < 0.0f) {
            int topTileRow = static_cast<int>(std::floor(entityY / TILE_SIZE));
            for(int currentCol = leftTileCol; currentCol <= rightTileCol; ++currentCol) {
                int tileID = gameWorld->getTileIdAT(currentCol, topTileRow);
                if(assets->isTileSolid(tileID)) {
                    entityY = (float)(topTileRow + 1) * TILE_SIZE;
                    entityVY = 0.0f;
                    return;}}}}                
                               
    int checkCollisionX(world* gameWorld, tileAssetManager *assets, float& entityX, float& entityY,
                        float& entityVX, float& entityVY, SDL_Rect& entityRect, bool& onGround, float& entityHeight, float& entityWidth){
        int tileSize = gameWorld->getTileSize();
        int topTile = static_cast<int>(std::floor(entityY / tileSize));
        int bottomTile = static_cast<int>(std::floor((entityY + entityHeight - 0.1f) / tileSize));

        if(entityVX > 0) {
            int R_coord = static_cast<int>(std::floor((entityX + entityWidth) / tileSize));
            for(int tileRow = topTile; tileRow <= bottomTile; tileRow++) {
                int tileID = gameWorld->getTileIdAT(R_coord, tileRow);
                if(assets->isTileSolid(tileID)) { 
                    entityX = (float)R_coord * tileSize - entityWidth;
                    entityVX = 0;
                    return 1;}}}
        if(entityVX < 0){
            int L_coord = static_cast<int>(std::floor(entityX / tileSize));
            for(int tileCol = topTile; tileCol <= bottomTile; tileCol++){
                int tileId = gameWorld->getTileIdAT(L_coord, tileCol);
                if(assets->isTileSolid(tileId)){
                    entityX = (float) (L_coord + 1) * tileSize;
                    entityVX = 0;
                    return -1;}}}
        return 0;        //this is to satisfy the compiler gods, no warnings on my watch                                                                                                                                     
    }
    
        //this is meant for the AI to not fall like morons to their death              
    int checkEdgeRight(world* gameWorld, tileAssetManager *assets, float& entityX, float& entityY,
                  float& entityVX, SDL_Rect& entityRect, float& entityHeight, float& entityWidth){     
        int tileSize = gameWorld->getTileSize();    
        float colBoxW = static_cast<float>(entityRect.w);
        float colBoxH = static_cast<float>(entityRect.h);                           
        int rightTileCol = static_cast<int>(std::floor((entityX + colBoxW - 0.1f) / tileSize));                                                                       
        int bottomTile = static_cast<int>(std::floor((entityY + entityHeight - 0.1f) / tileSize));
        int tileBelowRightEdge = gameWorld->getTileIdAT(rightTileCol, bottomTile + 1);
    
        if(!assets->isTileSolid(tileBelowRightEdge) && gameWorld->getTileIdAT(rightTileCol, bottomTile) != 9)
            if(entityY + colBoxH >= (float)(bottomTile +1) * tileSize -1.0f){
                entityVX = -GOOMBA_MOVE_SPEED;
                return 1;         //so, fun fact, this gives ILLEGAL INSTRUCTION while compilling. Damn, I am good.
            }
    }
                      
    int checkEdgeLeft(world* gameWorld, tileAssetManager *assets, float& entityX, float& entityY,
                      float& entityVX, SDL_Rect& entityRect, float& entityHeight, float& entityWidth){
        int tileSize = gameWorld->getTileSize();                          
        int leftTileCol = static_cast<int>(std::floor(entityX / tileSize));
        int bottomTile = static_cast<int>(std::floor((entityY + entityHeight - 0.1f) / tileSize));
        float colBoxH = static_cast<float>(entityRect.h); 
        int tileBelowLeftEdge = gameWorld->getTileIdAT(leftTileCol, bottomTile + 1);
        if(!assets->isTileSolid(tileBelowLeftEdge) && gameWorld->getTileIdAT(leftTileCol, bottomTile) != 9) 
            if(entityY + colBoxH >= (float)(bottomTile +1) * tileSize - 1.0f){
                entityVX = GOOMBA_MOVE_SPEED;
                return -1;      
            }
    }
};
#endif

