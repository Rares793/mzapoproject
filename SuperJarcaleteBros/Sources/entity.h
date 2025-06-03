#ifndef ENTITY_H
#define ENTITY_H

#include "utilities.h"
#include "world.h"

class entity{
public:
    virtual ~entity() = default;
    
    virtual void handleInput() = 0;
  
    virtual void loadTexture(SDL_Renderer* renderer) = 0;
  
    virtual void resolveMovement(world* gameWorld, tileAssetManager* assets, float deltaTime) = 0;
    
    virtual void destroy() = 0;
    
    virtual void render(SDL_Renderer* renderer, int cameraX, int cameraY) = 0;
    
    float& getX(){
        return entityX;
    }
                
    float& getY(){
        return entityY;
    }  
                
    float getEntityHeigth(){
        return entityHeight;
    }          
                
    float getEntityWidth(){
        return entityWidth;
    }
                
    SDL_Rect& getRect(){
        return entityRect;
    }
                
    float& getVX(){
        return entityVX;
    }
                
    float& getVY(){
        return entityVY;
    }            
         
    bool& getOG(){
        return onGround;
    }   
                
    void setDead(){
        alive = false;
    }    
protected:
    float entityX = 0.0f, entityY = 0.0f;
    float entityVY = 0.0f, entityVX = 0.0f;  
    float entityHeight = TEXTURE_HEIGHT;
    float entityWidth = TEXTURE_WIDTH;   
    bool onGround = true;
    bool alive = false;
public:
    uint16_t* entityTexture = nullptr;
    SDL_Rect entityRect = { (int)entityX, (int)entityY, TEXTURE_WIDTH, TEXTURE_HEIGHT };
};

#endif

