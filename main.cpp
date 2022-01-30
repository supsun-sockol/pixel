#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "matrix.h"

using namespace std;

int main()
{

    int matrix_w = 10;
    int matrix_h = 10;
    int pixel_w = 50;
    int border_count = 10;
    int fps = 10;

    int border_w = floor((float)pixel_w/(float)border_count);
    int w = matrix_w*(pixel_w+border_w)+border_w;
    int h = matrix_h*(pixel_w+border_w)+border_w;

    sf::RenderWindow window(sf::VideoMode(w, h), "pixel");
    window.setFramerateLimit(fps);
    sf::RenderTexture emptyTexture;
    emptyTexture.create(w, h);
    sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());
    sf::Shader shader;
    shader.loadFromFile("/home/supsun/Documents/pixel/shader.frag", sf::Shader::Fragment);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }



        }


        shader.setUniform("w", (float)w);
        shader.setUniform("h", (float)h);
        shader.setUniform("b_w", (float)border_w);
        shader.setUniform("b_c", (float)border_count);


        window.clear();
        window.draw(emptySprite, &shader);
        window.display();
    }
    return 0;
}
