#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameViewPlayer.h"
#include "CoinReader.h"

using namespace std;

class GameStateManager
{
public:
    sf::RectangleShape loadScreen;
    sf::Texture loadScreenTexture;
    sf::RenderWindow gameWindow;
    GameViewPlayer* playView;
    std::string currentState;
    CoinReader* coinReader;
    sf::Clock continueTimer;  // Countdown timer for continue screen
    
    GameStateManager();
    ~GameStateManager();
    virtual bool gameViewIsOpen();
    void setState(std::string state);
    void initialize();
    
    // Credit management methods
    int getCredits() const;
    bool hasCredits() const;
    bool useCredit();
    void checkForCoins();  // Poll coin reader, called every frame
    
    // View methods for new states
    bool insertCoinViewIsOpen();
    bool continueViewIsOpen();
};
