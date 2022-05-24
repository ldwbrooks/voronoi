#include "game.hpp"

const unsigned int SCREEN_WIDTH = 1500;
const unsigned int SCREEN_HEIGHT = 1000;


int main(int argc, char *argv[])
{
    ldwb::Game TheGame(SCREEN_WIDTH, SCREEN_HEIGHT);

    //initialize the game
    TheGame.Init();
    //run the game until an exit signal is received
    TheGame.RunLoop();
    return 0;
}