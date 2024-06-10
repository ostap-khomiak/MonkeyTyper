#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


struct Word {
    std::string string;
    float xPos;
    float yPos;
    bool onScreen = true;
    sf::Text word;


    Word(const std::vector<std::string>& words){
        xPos = 10.f;
        yPos = rand()%(600-100+1)+100;
        string = getRandomWord(words);
    }

    void update(float& timer, int& lives){
        if(xPos > 1300){
            onScreen = false;
            --lives;
        } else{
            xPos += 150.0f * timer;
        }
    }

    void drawWord(sf::Font font, sf::RenderWindow& window){
        word.setCharacterSize(18);
        word.setString(string);
        word.setFont(font);
        word.setFillColor(sf::Color(20,250,250));
        word.setPosition({xPos,yPos});

        word.setOutlineColor(sf::Color::Cyan);
        window.draw(word);
    }

    void changeColorOnUse(std::string& input){
        if((this->string).substr(0, input.size()) == input && input.size() != 0){
            this->word.setOutlineThickness(1);
        } else{
            this->word.setOutlineThickness(0);
        }
    }

    auto getRandomWord(const std::vector<std::string>& words) -> std::string;
};
