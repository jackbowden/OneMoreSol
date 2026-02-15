#include <SFML/Graphics.hpp>
#include "GameViewPlayer.h"
#include "GameLogic.h"
#include "GameStateManager.h"

using namespace std;

int main(int argc, char** argv)
{

    //sf::Event Event; // Process events

    string currentWinner;

    bool gameOver = false; // Never gets set to true
    bool quit = false; // Checks to see if player quit
    bool gameStarted = false;
    bool gameRestarted = false;

    GameStateManager* currentGameWindow = new GameStateManager();

    while(!gameOver) // Start game loop
    {
         // Start with Insert Coin screen (arcade mode)

        if(!gameStarted || gameRestarted)
        {
            currentGameWindow -> setState("InsertCoin");

            quit = currentGameWindow -> gameViewIsOpen();

            if(quit == true)
            {
                return 0;
            }
        }

        gameStarted = true;

        currentGameWindow -> setState("Play");

        quit = currentGameWindow -> gameViewIsOpen();
        gameRestarted = !quit;

        if (quit == true)
        {
            return 0;
        }

        // Player died - go to loss screen with arcade countdown
        currentGameWindow -> setState("Lost");

        gameRestarted = currentGameWindow -> gameViewIsOpen();

        if(currentGameWindow -> playView -> menuExit)
            return 0;

    }
}
