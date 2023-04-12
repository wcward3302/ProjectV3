#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "classes.cpp"


bool collision_detect (float, float, float, float, float, float, float, float);

int main (){

    // will use time to seed random generator for random wall heights
    srand(time(0));


    // create window with size of 1000 by 600, set framerate limit to 90fps, 
    sf::RenderWindow window (sf::VideoMode(2400, 1400), "Test");
    window.setFramerateLimit(60);


    // load sounds into buffer
    sf::SoundBuffer explode_buffer;
    sf::Sound explode;
    explode_buffer.loadFromFile("./Audio/explosion_x.wav");
    explode.setBuffer(explode_buffer);

    sf::SoundBuffer buzz_buffer;
    sf::Sound buzz;
    buzz_buffer.loadFromFile("./Audio/buzz.wav");
    buzz.setBuffer(buzz_buffer);


    

    // struct to hold textures for rendering, can just do sprite.settexture(texture.)
    struct Textures {
        sf::Texture ship[2]; // make ship have 2 textures, one for alive and one for dead
        sf::Texture wall;
        sf::Texture background;
    } textures;
    

    // load all images into members of Textures struct
    textures.ship[0].loadFromFile("./Images/ship.png");
    textures.ship[1].loadFromFile("./Images/explosion.png");
    textures.wall.loadFromFile("./Images/walls.png");
    textures.background.loadFromFile("./Images/deathstar_surface.jpeg");


    // create ship instance, set intial position (1/4 hori, 1/2 vert)
    Ship ship = Ship();
    ship.sprite.setPosition((window.getSize().x / 4) - (textures.ship->getSize().x), (window.getSize().y / 2) - (textures.ship->getSize().y / 2));

    // red outline around ship for testing
    sf::RectangleShape border;
    border.setFillColor(sf::Color::Red);

    // create a vector for the walls, so we can add and delete easily
    std::vector <sf::Sprite> walls;


    // create Game class instance and update
    Game game = Game();
    game.background.setTexture(textures.background);
    game.background.setScale(1.5, 1.5);
    game.font.loadFromFile("./Fonts/Menlo-Regular.ttf");

    game.score_text.setFont(game.font);
    game.score_text.setFillColor(sf::Color::White);
    game.score_text.setCharacterSize(50);
    game.score_text.setPosition(30, 30);

    game.high_score_text.setFont(game.font);
    game.high_score_text.setFillColor(sf::Color::White);
    game.high_score_text.setCharacterSize(50);
    game.high_score_text.setPosition(30, 90);

    game.enter_message_text.setFont(game.font);
    game.enter_message_text.setFillColor(sf::Color::White);
    game.enter_message_text.setCharacterSize(75);
    game.enter_message_text.setPosition(window.getSize().x / 2 - (window.getSize().x / 4), window.getSize().y / 2 - (window.getSize().y / 4));


    // will loop so long as window is open. Handle rendering and movement here
    while(window.isOpen()) {


        // load texts with current score and high score, but need to to_string them to load
        game.score_text.setString(std::to_string(game.score));
        game.high_score_text.setString(std::to_string(game.high_score));
        game.enter_message_text.setString(game.enter_message);


        // load ship sprite with initial texture unless game over, then set ship to dead
        if(game.game_state == 1){
            ship.sprite.setTexture(textures.ship[1]);
        }
        else{
            ship.sprite.setTexture(textures.ship[0]);
        }
        

        // track ship current position
        ship.x = ship.sprite.getPosition().x;
        ship.y = ship.sprite.getPosition().y;
        ship.width = textures.ship->getSize().x;
        ship.height =  textures.ship->getSize().y;
        sf::Vector2f border_size (ship.width, ship.height);
        border.setSize(border_size);
        border.setPosition(ship.x, ship.y);
        

        // if player goes out of bounds of screen
        if (game.game_state == 0) {
			if (ship.y < 0) {
				ship.sprite.setPosition(250, 0);
			} 
            else if (ship.y + ship.height > 2000) {
				game.game_state = 1;
				explode.play();
			}
		}
        

        // get score
		for (std::vector<sf::Sprite>::iterator itr = walls.begin(); itr != walls.end(); itr++) {
			if (game.game_state == 0 && (*itr).getPosition().x == 0 ){
				game.score++;
				buzz.play();

				if (game.score > game.high_score) {
					game.high_score = game.score;
				}
				break;
			}
		}


        // generate a new set of walls every 200 frames
        if (game.frames % 200 == 0){

            // use random generator for heights but restrict them to be 2x ship height apart. 
            int random_height = rand() % 850;
            int gap = textures.ship->getSize().y * 1.5;

            sf::Sprite wall_lower;
            wall_lower.setTexture(textures.wall);
            wall_lower.setPosition(2500, 0 - random_height);

            sf::Sprite wall_upper;
            wall_upper.setTexture(textures.wall);
            wall_upper.setPosition(2500, (0 - random_height) + textures.wall.getSize().y + gap);

            walls.push_back(wall_lower);
            walls.push_back(wall_upper);
            
        }

        int movement_factor = 1, speed;

        // move walls towards and past player
        if(game.game_state != 1){
            for (std::vector<sf::Sprite>::iterator itr = walls.begin(); itr != walls.end(); itr++) {
                
                (*itr).move(-10, 0);


                //(*itr).move(-(10 + (5 * (game.score / 10))), 0);
                //std::cout << "Speed = " << (10 + (5 * (game.score / 5))) << "\n";
			}
        }


        // collistion detection for game end
        if (game.game_state == 0){
            for (std::vector<sf::Sprite>::iterator itr = walls.begin(); itr != walls.end(); itr++) {
				
                // variables for coordinates of walls
                float wall_x, wall_y, wall_width, wall_height;


                // create variables to pass to collision function for each of the walls generated
                if((*itr).getScale().y > 0){
                    wall_x = (*itr).getPosition().x;
                    wall_y = (*itr).getPosition().y;
                    wall_width = textures.wall.getSize().x;
                    wall_height = textures.wall.getSize().y;
                }
                else{
                    wall_width = (*itr).getPosition().x;
                    wall_height = (*itr).getPosition().y;
                    wall_x = (*itr).getScale().x;
                    wall_y = (*itr).getScale().y;
                }


                // if collision is detected, set game to game over, explode
                if(collision_detect(ship.x, ship.y, ship.width, ship.height, wall_x, wall_y, wall_width, wall_height)){
                    game.game_state = 1;
                    explode.play();
                }
			}
        }

        int direction = 0;

        // check for keypresses to update ship position or restart
        sf::Event event;
        while (window.pollEvent(event)){
            
            if (event.type == sf::Event::Closed){
                window.close();
            }

            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                ship.sprite.setPosition((window.getSize().x / 4) - (textures.ship->getSize().x), (window.getSize().y / 2) - (textures.ship->getSize().y / 2));
                game.score = 0;
                walls.clear();
                game.game_state = 0;
            }

            if (game.game_state == 0){
                // keyboard input sets ships velocity which is always been applied, no input is 0 velocity
                if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
                    ship.velocity = 1; // velocity = up
                }

                else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down){
                    ship.velocity = 2; // velocity = down
                }
                else{
                    ship.velocity = 0; // velocity = 0
                }
            }
        }
    
        // clear screen, draw the background and ship sprite
        window.clear();
        window.draw(game.background);
        window.draw(border);

        // using new funciton .update for sprite, which takes the velocity set via keyboard input and moves the position of the sprite. 
        if(game.game_state == 0){
            ship.update();
        }
        
        window.draw(ship.sprite);
        
        // draw the walls
        for (std::vector<sf::Sprite>::iterator itr = walls.begin(); itr != walls.end(); itr++){
            window.draw(*itr);
        }

        if(game.frames < 150){
            window.draw(game.enter_message_text);
        }
        

        // draw scores
        window.draw(game.score_text);
        window.draw(game.high_score_text);
        

        // diplay and count frames
        window.display();
        game.frames++;

    }
    return 0;
}


// function to check for collision between 2 objects, given their x, y, width, height
bool collision_detect (float x_coord_1, float y_coord_1, float width_1, float height_1, float x_coord_2, float y_coord_2, float width_2, float height_2) {
	if (x_coord_1 + width_1 >= x_coord_2 && x_coord_1 <= x_coord_2 + width_2 && y_coord_1 + height_1 >= y_coord_2 && y_coord_1 <= y_coord_2 + height_2) {
		return true;
	}
	return false;
}