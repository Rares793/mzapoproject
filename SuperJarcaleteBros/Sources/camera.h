#ifndef CAMERA_H
#define CAMERA_H

#include "utilities.h"

class camera{
public:
    
    void update(float playerX, float playerY, float playerW, float playerH, float worldWidthPixels, float worldHeigthPixels, float deltaTime){
        float targetCamX = playerX - SCREEN_WIDTH / 2.0f + (playerW / 2.0f);
        float targetCamY = playerY + (playerH / 2.0f) - (SCREEN_HEIGHT / 2.0f);
        float smoothingFactorX = 0.01f; 
        float smoothingFactorY = 0.01f; 
 //       printf("\nInside Camera X: %f, Y%f\n", Player->getX(), Player->getY());
        //if (deltaTime > 0.0f) { 
          //  cameraX = lerp(cameraX, targetCamX, 1.0f - std::pow(smoothingFactorX, deltaTime));
            //cameraY = lerp(cameraY, targetCamY, 1.0f - std::pow(smoothingFactorY, deltaTime));
        //} else {
            cameraX = targetCamX;
            cameraY = targetCamY;
        //}
      
        if(cameraX < 0)
            cameraX = 0;
        if(cameraX > worldWidthPixels - SCREEN_WIDTH)
            cameraX = worldWidthPixels - SCREEN_WIDTH;
        
        if(cameraY < 0)
            cameraY = 0;
        if(cameraY > worldHeigthPixels - SCREEN_HEIGHT)
            cameraY = worldHeigthPixels - SCREEN_HEIGHT;
    }
    
   
    float lerp(float a, float b, float t){
         return(a + (b - a) * t);          
    }
                       
    float getX(){
        return cameraX;
    }
                
    float getY(){
        return cameraY;
    }            
private:
    float cameraX = 0, cameraY = 0;
    
};
#endif
