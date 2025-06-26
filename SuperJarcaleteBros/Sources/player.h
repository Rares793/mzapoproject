#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "collisionBox.h"
class player : public entity{
public:
    player(){
        entityX = 50.0f;
        entityY = SCREEN_HEIGHT / 2.0f;
        entityVX = 0.0f;
        entityVY = 0.0f;
        onGround = false;
        alive = true;
        }               
                                
    void loadTexture() override{
        std::ifstream fin("Assets/Worlds/World1/player16.txt");
        int numPixels = 16 * 16;
        entityTexture = new uint16_t[numPixels];
        std::string hexValue;
        int i = 0;
        while(fin >> hexValue && i < numPixels){
            entityTexture[i++] = static_cast<uint16_t>(std::stoul(hexValue, nullptr, 16));
            printf("%d ", entityTexture[i]);
        }
        entityRect.x = static_cast<int>(entityX);
        entityRect.y = static_cast<int>(entityY);
        entityWidth = static_cast<float>(entityRect.w); 
        entityHeight = static_cast<float>(entityRect.h); 
    }
                                   
    void handleInput() override {
        entityVX = 0.0f;

        uint32_t state = spiled_base[SPILED_REG_KNOBS_8BIT_o / 4];
        bool inputRight = (state >> 24) & 1;
        bool inputLeft = (state >> 25) & 1;
        bool inputJump = (state >> 26) & 1;
        if (inputLeft) 
            entityVX = -MOVE_SPEED;
        if (inputRight) 
            entityVX = MOVE_SPEED;
        if (inputJump){ 
            entityVY = JUMP_STRENGTH;
            onGround = 0;
        }
    }
                                                                                                                                             
    void resolveMovement(world* gameWorld, tileAssetManager* assets, float deltaTime) override{
        handleInput();
    
         if(!onGround) {
            entityVY += GRAVITY * deltaTime;
            if(entityVY > MAX_FALL_SPEED) entityVY = MAX_FALL_SPEED;
        }
        entityY += entityVY * deltaTime;
        onGround = false;
        CollisionBox.checkCollisionY(gameWorld, assets, entityX, entityY, entityVX, entityVY, entityRect, onGround);                                                   
        entityX += entityVX * deltaTime;
        CollisionBox.checkCollisionX(gameWorld, assets, entityX, entityY, entityVX, entityVY, entityRect, onGround, entityHeight, entityWidth);                                                                                       
        CollisionBox.checkCollisionWithWorld(gameWorld->getPixelWidth(), gameWorld->getPixelHeigth(), entityX, entityY, entityVX, entityVY, entityRect, onGround, alive);
        
        entityRect.x = static_cast<int>(entityX);
        entityRect.y = static_cast<int>(entityY);
    }
                                
    void bounceAfterStomp() {
        entityVY = -JUMP_STRENGTH * 0.6f;
        onGround = false; 
    }
                                                           
    void render(SDL_Renderer* renderer, int cameraX, int cameraY) override{
        SDL_Rect screenRect = {entityRect.x - cameraX, entityRect.y - cameraY, entityRect.w, entityRect.h};
        renderer->addToMemory(screenRect, entityTexture);
    }                              
                                                  
    void destroy() override{
        if (entityTexture) {
            delete[] entityTexture;
            entityTexture = nullptr;
        }
    }
          
    float getX(){
        return entityX;
    }                  
                   
    float getY(){
        return entityY;
    }             
    float getWidth(){
        return entityRect.w;
    }  
                                
    float getHeight(){
        return entityRect.h;
    }
private:
    collisionBox CollisionBox;
};
#endif

