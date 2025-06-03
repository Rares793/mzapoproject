 /*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  change_me.c      - main file

  include your name there and license for distribution.
  Vasile Ilie Rares Cristian, student at Technical University of Cluj Napoca
  Mario is owned by Nintendo, not me :))), but the code I guess is mine.

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include "Sources/game.h"
int main(int argc, char *argv[])
{

    /* Serialize execution of applications */

    /* Try to acquire lock the first */
    if (serialize_lock(1) <= 0) {
        printf("System is occupied\n");

        if (1) {
        printf("Waitting\n");
        /* Wait till application holding lock releases it or exits */
        serialize_lock(0);
        }
    }

    printf("Hello world\n");

    sleep(4);

    printf("Goodbye world\n");

    game* Game = new game();
    Game->init();
    Game->play();
    delete Game;
    return 0;
    
  /* Release the lock */
    serialize_unlock();

    return 0;
}
