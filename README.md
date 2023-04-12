# COP-project-game
Small time, not overly complex, basic game using C++ and SFML

<img width="1190" alt="Screenshot 2023-04-12 at 5 38 07 PM" src="https://user-images.githubusercontent.com/122639149/231594834-812202ef-cf23-4302-9355-bfaf6c86af0d.png">

## Note: 
not in final state, some more finishing touches are required but it 95% complete.
still missing:
- increase speed based on score
- adjust ship hitbox for better accuracy
- touch up comments and code for readability

## For: COP-3003 course project

## Reason: 
what initially started as an idea to create a game launcher with multiple different games, I decided early on that trying SFML with C++ would provide a greater challenge. So I came up witha goal for a game that almost all components I made myself. The ship and walls seen were drawn by yours truly. The only component sourced from the internet is the sounds, the fonts, and the background. Everything else was made in house. 

## Functionality: 
- displays a window
- framerate locked at 60 frame/sec
- takes input from the keyboard to adjust velocity of the ship
- generates and displays the walls at random heights
- as player passes a pair of walls, score increases, and high score is adjusted

## Maintainers
Yours truly

## How to run on MacOS

- brew install sfml
- g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
- ./a.out
