#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

int x=400;
int y=400;

int main(){

    cout << "main()" << endl;

    RenderWindow window(VideoMode(800,600), "Game 1");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            // "close requewsted" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (Keyboard::isKeyPressed(Keyboard::Up))
                    y -= 10;
            if (Keyboard::isKeyPressed(Keyboard::Down))
                    y += 10;
            if (Keyboard::isKeyPressed(Keyboard::Right))
                    x += 10;
            if (Keyboard::isKeyPressed(Keyboard::Left))
                    x -= 10;
        }


        // Initiate window with color
        window.clear(Color().Blue);

        // Declare and load a texture
        Texture texture;
        texture.loadFromFile("images/ball1.png");

        // Create a ball
        CircleShape ball;
        ball.setPosition(x,y);
        ball.setFillColor(Color().Magenta);
        ball.setRadius(50);
        window.draw(ball);


        // Create a sprite
        Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(IntRect(50,50,50,50));
        sprite.setColor(Color(255,255,255,200));
        sprite.setPosition(100,25);
        
        window.draw(sprite);
        

        window.display();
    }

    return 0;
}