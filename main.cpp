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
    float createTimer = 0.0f;

    auto font = sf::Font();
    if(!font.loadFromFile("../arial.ttf")) {
        return EXIT_FAILURE;
    }

    std::string input;
    sf::Text inputText;
    inputText.setPosition(100,650);
    inputText.setFillColor(sf::Color::Blue);
    inputText.setFont(font);
    inputText.setCharacterSize(32);

    auto window = sf::RenderWindow(sf::VideoMode(1280, 720), "Monkey Typer");

    std::vector<std::string> words = getWords("../words1k.txt");
    std::vector<Word> activeWords;


    while (window.isOpen()) {
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if( event.type == sf::Event::TextEntered){
                if(event.text.unicode == '\b' && !input.empty()){
                    input.pop_back();
                } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) &&  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
                    input.clear();
                } else if(event.text.unicode == '\r'){
                    for(auto& word : activeWords){
                        if(word.string == input){
                            word.onScreen = false;
                            break;
                        }
                    }
                    input.clear();
                } else if (event.text.unicode != '\b' && event.text.unicode != ' '){
                    input += static_cast<char>(event.text.unicode);
                }
                inputText.setString(input);
            }
        }


        float createInterval = (rand() % 3)+1;

        float timer = clock.restart().asSeconds();
        createTimer += timer;
        if (createTimer >= createInterval && activeWords.size() < 8){
            createTimer = 0.0f;
            Word word(words);
            activeWords.push_back(word);
        }

        for(auto& word : activeWords){
            word.update(timer);
        }

        for (auto& word : activeWords) {
            if (input == word.string.substr(0, input.size())) {
                word.setColor(sf::Color::Green);
            } else {
                word.setColor(sf::Color::Yellow);
            }
        }

        auto it = activeWords.begin();
        while (it != activeWords.end()) {
            if (!it->onScreen) {
                it = activeWords.erase(it);
            } else {
                ++it;
            }
        }

        window.clear(sf::Color::Black);

        for(auto& word : activeWords){
            word.drawWord(font, window);
        }

        window.draw(inputText);

        window.display();
    }

    return EXIT_SUCCESS;
}