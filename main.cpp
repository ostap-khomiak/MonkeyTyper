#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include "word.h"


std::vector<std::string> getWords(const std::string& fname){
    std::vector<std::string> words;
    std::ifstream file(fname);

    std::string word;
    if(file) {
        while(file >> word)
            words.push_back(word);
    }
    return words;
}


auto main() -> int{
    srand(time(NULL));
    sf::Clock clock;

    float elapsed;
    int wpm;
    float createTimer = 0.0f;

    bool showMenu = true;
    bool isRunning = false;

    auto arialFont = sf::Font();
    if(!arialFont.loadFromFile("../arial.ttf")) {
        return EXIT_FAILURE;
    }

    auto comicFont = sf::Font();
    if(!comicFont.loadFromFile("../comic.ttf")) {
        return EXIT_FAILURE;
    }

    auto witcherFont = sf::Font();
    if(!witcherFont.loadFromFile("../witcher.ttf")) {
        return EXIT_FAILURE;
    }

    auto defaultFont = arialFont;

    int lives;
    sf::Text livesText;
    livesText.setPosition(1050,50);
    livesText.setFillColor(sf::Color::Cyan);
    livesText.setFont(arialFont);
    livesText.setCharacterSize(24);

    int score = 0;

    sf::Text countWordsText;
    countWordsText.setPosition(800, 650);
    countWordsText.setFillColor(sf::Color::Cyan);
    countWordsText.setFont(arialFont);
    countWordsText.setCharacterSize(24);

    sf::Text wpmText;
    wpmText.setPosition(1050,650);
    wpmText.setFillColor(sf::Color::Cyan);
    wpmText.setFont(arialFont);
    wpmText.setCharacterSize(24);

    std::string input;
    sf::Text inputText;
    inputText.setPosition(50,650);
    inputText.setFillColor(sf::Color::Cyan);
    inputText.setFont(arialFont);
    inputText.setCharacterSize(24);

    sf::Text goToMainMenu;
    goToMainMenu.setString("Go to Menu");
    goToMainMenu.setFont(arialFont);
    goToMainMenu.setFillColor(sf::Color(100,100,100));
    goToMainMenu.setCharacterSize(50);
    goToMainMenu.setPosition(510, 400);

    sf::Text playButton;
    playButton.setString("PLAY");
    playButton.setFont(arialFont);
    playButton.setCharacterSize(50);
    playButton.setPosition(575,300);
    playButton.setFillColor(sf::Color(0,220,220));

    sf::Text difficultyText;
    difficultyText.setString("Difficulty:");
    difficultyText.setFont(arialFont);
    difficultyText.setCharacterSize(30);
    difficultyText.setPosition(50,400);
    difficultyText.setFillColor(sf::Color(100,100,100));

    sf::Text fontText;
    fontText.setString("Font:");
    fontText.setFont(arialFont);
    fontText.setCharacterSize(30);
    fontText.setPosition(50,500);
    fontText.setFillColor(sf::Color(100,100,100));

    sf::Text comicButton;
    comicButton.setString("Comic");
    comicButton.setFont(comicFont);
    comicButton.setCharacterSize(30);
    comicButton.setPosition(290,500);
    comicButton.setOutlineColor(sf::Color::Cyan);
    comicButton.setOutlineThickness(0);

    sf::Text arialButton;
    arialButton.setString("Arial");
    arialButton.setFont(arialFont);
    arialButton.setCharacterSize(30);
    arialButton.setPosition(600,500);
    arialButton.setOutlineColor(sf::Color::Cyan);
    arialButton.setOutlineThickness(2);

    sf::Text witcherButton;
    witcherButton.setString("Witcher");
    witcherButton.setFont(witcherFont);
    witcherButton.setCharacterSize(30);
    witcherButton.setPosition(900,500);
    witcherButton.setOutlineColor(sf::Color::Cyan);
    witcherButton.setOutlineThickness(0);


    sf::Text easyButton;
    easyButton.setString("easy");
    easyButton.setFont(arialFont);
    easyButton.setCharacterSize(30);
    easyButton.setPosition(300,400);
    easyButton.setOutlineColor(sf::Color::Cyan);
    easyButton.setOutlineThickness(0);

    sf::Text mediumButton;
    mediumButton.setString("medium");
    mediumButton.setFont(arialFont);
    mediumButton.setCharacterSize(30);
    mediumButton.setPosition(580,400);
    mediumButton.setOutlineColor(sf::Color::Cyan);
    mediumButton.setOutlineThickness(2);

    sf::Text hardButton;
    hardButton.setString("hard");
    hardButton.setFont(arialFont);
    hardButton.setCharacterSize(30);
    hardButton.setPosition(920,400);
    hardButton.setOutlineColor(sf::Color::Cyan);
    hardButton.setOutlineThickness(0);


    auto window = sf::RenderWindow(sf::VideoMode(1280, 720), "Monkey Typer");
    window.setFramerateLimit(60);


    std::vector<std::string> words = getWords("../medium.txt");
    std::vector<Word> activeWords;

    while (window.isOpen()) {
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::TextEntered: {
                    if (event.text.unicode == '\b' && !input.empty()) {
                        input.pop_back();
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) &&
                               sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        input.clear();
                    } else if (event.text.unicode == ' ' || event.text.unicode == '\r') {
                        for (auto &word: activeWords) {
                            if (word.string == input) {
                                word.onScreen = false;
                                ++score;
                                break;
                            }
                        }
                        input.clear();
                    } else if (event.text.unicode != '\b') {
                        input += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(input);
                    for(auto& word : activeWords){
                        word.changeColorOnUse(input);
                    }
                }
                case sf::Event::MouseButtonPressed: {
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    if (goToMainMenu.getGlobalBounds().contains(mousePosF ) && !isRunning && !showMenu) {
                        isRunning = false;
                        showMenu = true;
                    } else if( playButton.getGlobalBounds().contains(mousePosF) && showMenu) {
                        activeWords.clear();
                        score = 0;
                        input.clear();
                        lives = 3;
                        isRunning = true;
                        showMenu = false;
                        clock.restart();
                    } else if(mediumButton.getGlobalBounds().contains(mousePosF) && showMenu){
                        words.clear();
                        words = getWords("../medium.txt");
                        mediumButton.setOutlineThickness(2);
                        easyButton.setOutlineThickness(0);
                        hardButton.setOutlineThickness(0);
                    } else if(easyButton.getGlobalBounds().contains(mousePosF) && showMenu){
                        words.clear();
                        words = getWords("../easy.txt");
                        mediumButton.setOutlineThickness(0);
                        easyButton.setOutlineThickness(2);
                        hardButton.setOutlineThickness(0);
                    } else if(hardButton.getGlobalBounds().contains(mousePosF) && showMenu){
                        words.clear();
                        words = getWords("../hard.txt");
                        mediumButton.setOutlineThickness(0);
                        easyButton.setOutlineThickness(0);
                        hardButton.setOutlineThickness(2);
                    } else if(comicButton.getGlobalBounds().contains(mousePosF) && showMenu) {
                        defaultFont = comicFont;
                        comicButton.setOutlineThickness(2);
                        arialButton.setOutlineThickness(0);
                        witcherButton.setOutlineThickness(0);
                    } else if(arialButton.getGlobalBounds().contains(mousePosF) && showMenu) {
                        defaultFont = arialFont;
                        comicButton.setOutlineThickness(0);
                        arialButton.setOutlineThickness(2);
                        witcherButton.setOutlineThickness(0);
                    } else if(witcherButton.getGlobalBounds().contains(mousePosF) && showMenu) {
                        defaultFont = witcherFont;
                        comicButton.setOutlineThickness(0);
                        arialButton.setOutlineThickness(0);
                        witcherButton.setOutlineThickness(2);
                    }
                }
            }
        }


        if(showMenu && !isRunning){
            sf::Text menu;
            menu.setCharacterSize(62);
            menu.setPosition(440,100);
            menu.setFont(arialFont);
            menu.setFillColor(sf::Color::White);
            menu.setString("Monkey Typer");
            menu.setFillColor(sf::Color::Cyan);
            window.clear(sf::Color::Black);
            window.draw(menu);
            window.draw(difficultyText);
            window.draw(playButton);
            window.draw(easyButton);
            window.draw(mediumButton);
            window.draw(hardButton);
            window.draw(fontText);
            window.draw(comicButton);
            window.draw(arialButton);
            window.draw(hardButton);
            window.draw(witcherButton);
        }

        if(lives == 0 && !showMenu){
            isRunning = false;
            sf::Text gameOver;
            gameOver.setCharacterSize(62);
            gameOver.setPosition(480,100); // center text
            gameOver.setFont(arialFont);
            gameOver.setFillColor(sf::Color::White);
            gameOver.setString("Game Over");
            window.clear(sf::Color::Black);
            window.draw(gameOver);
            window.draw(goToMainMenu);
        }

        if (isRunning && !showMenu){

            countWordsText.setString("Score: " + std::to_string(score));
            livesText.setString("Lives: " + std::to_string(lives));

            float createInterval = (rand() % 4)+1;
            elapsed += clock.getElapsedTime().asSeconds();
            float timer = clock.restart().asSeconds();
            createTimer += timer;
            if (createTimer >= createInterval && activeWords.size() < 8){
                createTimer = 0.0f;
                Word word(words);
                activeWords.push_back(word);
            }

            for(auto& word : activeWords){
                word.update(timer, lives);
            }



            auto it = activeWords.begin();
            while (it != activeWords.end()) {
                if (!it->onScreen) {
                    it = activeWords.erase(it);
                    break;
                } else {
                    ++it;
                }
            }

            window.clear(sf::Color::Black);

            for(auto& word : activeWords){
                word.drawWord(defaultFont, window);
            }

            wpm = score / elapsed * 60;
            wpmText.setString("WPM: "+std::to_string(wpm));

            window.draw(livesText);
            window.draw(inputText);
            window.draw(countWordsText);
            window.draw(wpmText);

        }
        window.display();
    }

    return EXIT_SUCCESS;
}