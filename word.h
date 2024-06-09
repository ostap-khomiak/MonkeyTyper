#pragma once

#include <SFML/Graphics.hpp>



struct Word {
    std::string string;
    float xPos;
    float yPos;
    bool onScreen = true;
    sf::Text word;


    Word(const std::vector<std::string>& words){
        xPos = 10.f;
        yPos = rand()%(550-100+1)+100;
        string = getRandomWord(words);
    }

    void update(float timer){
        if(xPos > 1300){
            onScreen = false;
        } else{
            xPos += 150.0f * timer;
        }
    }

    void drawWord(sf::Font font, sf::RenderWindow& window){
        word.setCharacterSize(18);
        word.setString(string);
        word.setFont(font);
        word.setFillColor(sf::Color::Yellow);
        word.setPosition({xPos,yPos});
        window.draw(word);
    }

    void setColor(sf::Color color){
        word.setFillColor(color);
    }

    auto getRandomWord(const std::vector<std::string>& words) -> std::string;
};
