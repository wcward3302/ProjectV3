#include <SFML/Graphics.hpp>

// class for ship, holds basic information about ship position and size
class Ship {

public:
    double velocity;
    double x;
    double y;
    double height;
    double width;

    sf::Sprite sprite;

    Ship()=default;

    void update(){
        if (velocity == 1){
            sprite.move(0,-15);
        }
        else if (velocity == 2){
            sprite.move(0,15);
        }
        else{
            sprite.move(0,0);
        }
    }

};


// game class holds information about game instance, like the score and high score, as well as a frame counter variable
class Game {

public:
    int score;
    int high_score;
    int frames;
    std::string enter_message = "Welcome!\nUse arrow keys to move up and down\nMay the mass x acceleration be with you...";
    int game_state; // 0 is running, 1 is stopped

    sf::Sprite background;
    sf::Text score_text;
    sf::Text high_score_text;
    sf::Text enter_message_text;
    sf::Font font;

    Game()=default;

};