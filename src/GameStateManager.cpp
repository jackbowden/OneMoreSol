#include "GameStateManager.h"
#include <iostream>

GameStateManager::GameStateManager()
: gameWindow(sf::VideoMode(1440, 900, 32), "One More Sol",sf::Style::Titlebar | sf::Style::Close)
{
    // Initialize coin reader based on platform
    #ifdef __APPLE__
        coinReader = new KeyboardCoinReader();
    #else
        coinReader = new SerialCoinReader("/dev/ttyUSB0");
    #endif
    
    initialize();
}

GameStateManager::~GameStateManager()
{
    delete coinReader;
    delete playView;
}

void GameStateManager::initialize()
{
    if(!loadScreenTexture.loadFromFile("../assets/loadingScreen.png"))
        std::cout<<"Failed to load loading screen." << std::endl;
    loadScreen.setOrigin(0,900);
    loadScreen.setPosition(0,900);
    loadScreen.setSize(sf::Vector2f(1440,900));
    loadScreen.setTexture(&loadScreenTexture);
    gameWindow.draw(loadScreen);
    gameWindow.display();
    playView = new GameViewPlayer();
}

bool GameStateManager::gameViewIsOpen()
{
    bool quit;

    loadScreen.setSize(sf::Vector2f(0,0));//quick fix for loading screen not going away after menu is switched to.
    
    // Always check for coins (non-blocking)
    checkForCoins();

    if (currentState == "InsertCoin")
    {
        quit = insertCoinViewIsOpen();
        return quit;
    }
    
    if (currentState == "Start")
    {
        quit = playView -> menuViewIsOpen(gameWindow);
        return quit;
    }
    if (currentState == "Play")
    {
        quit = playView -> gameViewIsOpen(gameWindow);
        return quit;
    }

    if (currentState == "Lost")
    {
        if(playView -> returnToMenu)
          quit = playView -> menuViewIsOpen(gameWindow);
        else
          quit = playView -> lossViewIsOpen(gameWindow, coinReader);  // Pass coin reader for arcade mode
        return quit;
    }
    
    return false;
}

void GameStateManager::setState(std::string state)
{
    currentState = state;
}

// ============================================================================
// Credit Management Methods
// ============================================================================

int GameStateManager::getCredits() const
{
    return coinReader->getCredits();
}

bool GameStateManager::hasCredits() const
{
    return coinReader->getCredits() > 0;
}

bool GameStateManager::useCredit()
{
    return coinReader->useCredit();
}

void GameStateManager::checkForCoins()
{
    coinReader->checkForCoin();
}

// ============================================================================
// Insert Coin View (Attract Mode)
// ============================================================================

bool GameStateManager::insertCoinViewIsOpen()
{
    // Start menu music
    playView->gameMusic.stop();
    playView->gameMusic.setBuffer(playView->loadedAudio->soundTrack[20]);
    playView->gameMusic.play();
    playView->gameMusic.setLoop(true);

    sf::Event event;
    sf::Clock blinkClock;
    bool blinkOn = true;
    int menuSelection = 0;  // 0 = Play, 1 = Story, 2 = Exit

    // Initialize menu selection to highlight Play button
    playView->selectMenuButton(gameWindow, menuSelection);

    sf::Font overlayFont;
    bool hasFont = overlayFont.loadFromFile("../assets/impact.ttf");
    if (!hasFont)
    {
        hasFont = overlayFont.loadFromFile("assets/impact.ttf");
    }

    sf::RectangleShape dimmer(sf::Vector2f(1440, 900));
    dimmer.setPosition(0, 0);
    dimmer.setFillColor(sf::Color(0, 0, 0, 150));

    sf::RectangleShape panel(sf::Vector2f(760, 300));
    panel.setPosition(340, 200);
    panel.setFillColor(sf::Color(20, 20, 20, 230));
    panel.setOutlineColor(sf::Color(255, 215, 0));
    panel.setOutlineThickness(4);

    sf::Text title;
    sf::Text subtitle;
    sf::Text creditText;
    sf::Text helper;
    if (hasFont)
    {
        title.setFont(overlayFont);
        subtitle.setFont(overlayFont);
        creditText.setFont(overlayFont);
        helper.setFont(overlayFont);

        title.setCharacterSize(68);
        subtitle.setCharacterSize(40);
        creditText.setCharacterSize(44);
        helper.setCharacterSize(28);

        title.setFillColor(sf::Color(255, 230, 80));
        subtitle.setFillColor(sf::Color::White);
        creditText.setFillColor(sf::Color::Cyan);
        helper.setFillColor(sf::Color(190, 190, 190));
    }

    while (gameWindow.isOpen())
    {
        while (gameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                gameWindow.close();
                return true;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    menuSelection = (menuSelection - 1 + 3) % 3;
                    playView->selectMenuButton(gameWindow, menuSelection);
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    menuSelection = (menuSelection + 1) % 3;
                    playView->selectMenuButton(gameWindow, menuSelection);
                }
                else if (event.key.code == sf::Keyboard::Return)
                {
                    if (menuSelection == 0)  // Play
                    {
                        if (hasCredits())
                        {
                            useCredit();
                            setState("Play");
                            return false;
                        }
                    }
                    else if (menuSelection == 1)  // Story
                    {
                        playView->storyViewIsOpen(gameWindow);
                    }
                    else if (menuSelection == 2)  // Exit
                    {
                        return true;
                    }
                }
            }
        }

        // Check for coins (handles 'C' key press automatically)
        checkForCoins();

        // Update button text based on credits
        if (hasCredits())
        {
            playView->playButtonText.setString("Play (" + std::to_string(getCredits()) + ")");
        }
        else
        {
            playView->playButtonText.setString("Insert Credit");
        }
        sf::FloatRect textBounds = playView->playButtonText.getLocalBounds();
        playView->playButtonText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
        playView->playButtonText.setPosition(1440 - (1308/2)/2.0f, 400 + (224/2)/2.0f - 8);

        if (blinkClock.getElapsedTime().asSeconds() > 0.5f)
        {
            blinkOn = !blinkOn;
            blinkClock.restart();
        }

        // Space as shortcut to play if credits available
        if (hasCredits() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            useCredit();
            setState("Play");
            return false;
        }

        gameWindow.clear(sf::Color::Black);
        playView->drawMenuBackdrop(gameWindow);
        // Dialog box removed - credit info now shows on the button

        gameWindow.display();
    }

    return true;
}

// ============================================================================
// Continue Countdown Screen
// ============================================================================

bool GameStateManager::continueViewIsOpen()
{
    sf::Event event;

    sf::Font overlayFont;
    bool hasFont = overlayFont.loadFromFile("../assets/impact.ttf");
    if (!hasFont)
    {
        hasFont = overlayFont.loadFromFile("assets/impact.ttf");
    }

    sf::RectangleShape dimmer(sf::Vector2f(1440, 900));
    dimmer.setPosition(0, 0);
    dimmer.setFillColor(sf::Color(0, 0, 0, 170));

    sf::RectangleShape panel(sf::Vector2f(900, 300));
    panel.setPosition(270, 580);
    panel.setFillColor(sf::Color(20, 20, 20, 240));
    panel.setOutlineColor(sf::Color(255, 80, 80));
    panel.setOutlineThickness(4);

    sf::Text gameOverText;
    sf::Text continueText;
    sf::Text timerText;
    sf::Text creditsText;
    sf::Text helperText;
    if (hasFont)
    {
        gameOverText.setFont(overlayFont);
        continueText.setFont(overlayFont);
        timerText.setFont(overlayFont);
        creditsText.setFont(overlayFont);
        helperText.setFont(overlayFont);

        gameOverText.setCharacterSize(72);
        continueText.setCharacterSize(48);
        timerText.setCharacterSize(110);
        creditsText.setCharacterSize(44);
        helperText.setCharacterSize(28);

        gameOverText.setFillColor(sf::Color(255, 90, 90));
        continueText.setFillColor(sf::Color::White);
        timerText.setFillColor(sf::Color(255, 230, 120));
        creditsText.setFillColor(sf::Color::Cyan);
        helperText.setFillColor(sf::Color(200, 200, 200));
    }
    
    // Start 30 second countdown
    continueTimer.restart();
    const float countdownDuration = 30.0f;
    
    while (gameWindow.isOpen())
    {
        while (gameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                gameWindow.close();
                return true;  // Quit
            }
        }
        
        // Check for coins (handles 'C' key press automatically)
        checkForCoins();
        
        // Calculate remaining time
        float elapsed = continueTimer.getElapsedTime().asSeconds();
        int secondsLeft = static_cast<int>(countdownDuration - elapsed);
        
        if (secondsLeft < 0)
        {
            secondsLeft = 0;
        }
        
        // Check if coin inserted
        if (hasCredits())
        {
            useCredit();
            // Continue game at current level (don't reset progress)
            setState("Play");
            return false;
        }
        
        // Check if countdown expired
        if (elapsed >= countdownDuration)
        {
            // No credits, return to insert coin screen and reset game
            playView->resetGame();
            setState("InsertCoin");
            return true;  // Signal to main loop to restart from InsertCoin
        }
        
        // Render
        gameWindow.clear(sf::Color::Black);
        gameWindow.draw(playView->lossScreen);
        gameWindow.draw(dimmer);
        gameWindow.draw(panel);

        if (hasFont)
        {
            gameOverText.setString("GAME OVER");
            sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
            gameOverText.setPosition(720 - gameOverBounds.width / 2.0f, 600);

            continueText.setString("INSERT CREDIT TO CONTINUE");
            sf::FloatRect continueBounds = continueText.getLocalBounds();
            continueText.setPosition(720 - continueBounds.width / 2.0f, 675);

            timerText.setString(std::to_string(secondsLeft));
            sf::FloatRect timerBounds = timerText.getLocalBounds();
            timerText.setPosition(720 - timerBounds.width / 2.0f, 735);

            creditsText.setString("CREDITS: " + std::to_string(getCredits()));
            sf::FloatRect creditsBounds = creditsText.getLocalBounds();
            creditsText.setPosition(720 - creditsBounds.width / 2.0f, 835);

            helperText.setString("C = INSERT CREDIT");
            sf::FloatRect helperBounds = helperText.getLocalBounds();
            helperText.setPosition(720 - helperBounds.width / 2.0f, 870);

            gameWindow.draw(gameOverText);
            gameWindow.draw(continueText);
            gameWindow.draw(timerText);
            gameWindow.draw(creditsText);
            gameWindow.draw(helperText);
        }
        else
        {
            sf::RectangleShape fallbackHeadline(sf::Vector2f(680, 90));
            fallbackHeadline.setPosition(380, 615);
            fallbackHeadline.setFillColor(sf::Color(160, 40, 40));
            gameWindow.draw(fallbackHeadline);

            sf::RectangleShape fallbackTimerBox(sf::Vector2f(220, 140));
            fallbackTimerBox.setPosition(610, 725);
            fallbackTimerBox.setFillColor(sf::Color(40, 40, 40));
            fallbackTimerBox.setOutlineColor(sf::Color(255, 230, 120));
            fallbackTimerBox.setOutlineThickness(3);
            gameWindow.draw(fallbackTimerBox);

            sf::RectangleShape fallbackProgressBg(sf::Vector2f(520, 30));
            fallbackProgressBg.setPosition(460, 870);
            fallbackProgressBg.setFillColor(sf::Color(45, 45, 45));
            fallbackProgressBg.setOutlineColor(sf::Color::White);
            fallbackProgressBg.setOutlineThickness(2);
            gameWindow.draw(fallbackProgressBg);

            float remainingPercent = (countdownDuration - elapsed) / countdownDuration;
            if (remainingPercent < 0.f)
            {
                remainingPercent = 0.f;
            }
            sf::RectangleShape fallbackProgress(sf::Vector2f(516.f * remainingPercent, 26));
            fallbackProgress.setPosition(462, 872);
            fallbackProgress.setFillColor(sf::Color::Yellow);
            gameWindow.draw(fallbackProgress);
        }

        // Progress bar removed to save space

        gameWindow.display();
    }
    
    return true;  // Window closed
}
