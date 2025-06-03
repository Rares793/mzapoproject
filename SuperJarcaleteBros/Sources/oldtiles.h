//Really stupid and naive implementation of tile rendering.
//Has been discontinued in favour of the objectively superior Sprite Sheet
//Only the functions have been saved because why not?
void initialiseTexture(SDL_Renderer* renderer, string id){
        std::string filePath = "Assets/object";
        filePath += id + ".png";
        SDL_Surface* loadedSurface = IMG_Load(filePath);
        int ID = toInt(id);
        loadedTiles[ID] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
               
    void initialiseAllTextures(SDL_Renderer* renderer, int* ids, int howMany){
        for(int i = 0; i < howMany; i++){
            std::string stringId = std::to_string(ids[i]);
            initialiseTexture(renderer, stringId);
        }
    }  
                         
    void assignTextureToProperty(SDL_Renderer* renderer, string id){
        int ID = toInt(id);
        tile[ID]->tileTexture = loadedTiles[ID];
        SDL_QueryTexture(tile[ID]->tileTexture, NULL, NULL, &playerRect.w, &playerRect.h);
        tile[ID]->tileY = SCREEN_HEIGHT - tile[ID]->tileRect.h;
        tile[ID]->tileRect.y = (int)tile[ID]->tileY;
    }

    void initialiseTiles(SDL_Renderer* renderer, std::string tileMap, int tileMapWidth, int tileMapHeigth, int tileMapSize){
        int size = tileMapWidth * tileMapHeigth * sizeof(TileObject)
        tile = (tileObject*)malloc(size);
        for(int i = 0; i < tileMapSize; i++){
            std::string stringId = std::to_string(i);
            assignTextureToProperty(renderer, stringId);
            if(tileMap[i] != 9)
                 tile[i].isSolid = 1
        }
    } 
    
    void initialise(SDL_Renderer* renderer, std::string tileMap, int tileMapWidth, int tileMapHeigth, int tileMapSize, int* ids, int howMany){
        initialiseAllTextures (renderer, ids, howMany);
        initialiseTiles(renderer, tileMap, tileMapWidth, tileMapHeigth, tileMapSize);
    }           )
