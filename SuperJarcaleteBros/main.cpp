#include "Sources/game.h"

int main(int argc, char* args[]) {
    game* Game = new game();
    Game->init();
    Game->play();
    delete Game;
    return 0;
}
