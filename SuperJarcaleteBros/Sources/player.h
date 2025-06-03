#ifndef PLAYER_H
#define PLAYER_h

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
                                
    void loadTexture(SDL_Renderer* renderer) override{
        std::ifstream fin("Assets/Worlds/World1/player16.txt");
        int numPixels = 16 * 16;
        entityTexture = new uint16_t[numPixels];
        std::string hexValue;
        int i = 0;
        while(fin >> hexValue && i < numPixels)
            entityTexture[i++] = static_cast<uint16_t>(std::stoul(hexValue, nullptr, 16));
        
        entityRect.x = static_cast<int>(entityX);
        entityRect.y = static_cast<int>(entityY);
        entityWidth = static_cast<float>(entityRect.w); 
        entityHeight = static_cast<float>(entityRect.h); 
    }
                                
    bool is_left_pressed() {
        uint32_t val = spiled_base[SPILED_REG_KBDRD_KNOBS_DIRECT_o / 4];
        return val & (1 << 26);
    }

    bool is_middle_pressed() {
        uint32_t val = spiled_base[SPILED_REG_KBDRD_KNOBS_DIRECT_o / 4];
        return val & (1 << 27);
    }

    bool is_right_pressed() {
        uint32_t val = spiled_base[SPILED_REG_KBDRD_KNOBS_DIRECT_o / 4];
        return val & (1 << 28);
    }
                                   
    void handleInput() override {
        entityVX = 0.0f;

        if (is_middle_pressed()) 
            entityVX = -MOVE_SPEED;
        if (is_right_pressed()) 
            entityVX = MOVE_SPEED;
        if (is_left_pressed()){ 
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
        renderer.addToMemory(screenRect, entityTexture);
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

