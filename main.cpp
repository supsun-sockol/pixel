#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "matrix.h"
#include <random>

int matrix_w = 100;
int matrix_h = 100;
int pixel_w = 5;
int border_count = 10;
int fps = 50;
int border_w = floor((float)pixel_w/(float)border_count);
int w = matrix_w*(pixel_w+border_w)+border_w;
int h = matrix_h*(pixel_w+border_w)+border_w;


float* GetShaderArray(matrix<int>& m, int min_x, int max_x, int min_y, int max_y){
    float* arr = new float[(max_x-min_x)*(max_y-min_y)];
    for (int y = min_y; y<max_y; y++){
        for (int x = min_x; x<max_x; x++){
            arr[y*(max_x-min_x)+x-min_x] = m.get(x, y);
        }
    }
    return arr;
}

void DarwMatrix(matrix<int>& m, int min_x, int max_x, int min_y, int max_y, sf::RenderWindow& window, sf::Shader& drawshader){
    if((max_x-min_x)*(max_y-min_y)<=10000){
        sf::RenderTexture drawTexture;
        drawTexture.create(w, h);
        sf::Sprite draw = sf::Sprite(drawTexture.getTexture());
        drawshader.setUniform("w", (float)matrix_w);
        drawshader.setUniform("h", (float)matrix_h);
        drawshader.setUniformArray("arr", GetShaderArray(m, 0, matrix_w, 0, matrix_h) , matrix_w*matrix_h);
        window.draw(draw, &drawshader);
    }
}

int CountAround(matrix<int> m, int x, int y){
    int s = 0;
    s+=m.get(x+1, y+1);
    s+=m.get(x+1, y);
    s+=m.get(x+1, y-1);
    s+=m.get(x, y-1);
    s+=m.get(x-1, y-1);
    s+=m.get(x-1, y);
    s+=m.get(x-1, y+1);
    s+=m.get(x, y+1);
    return s;
}

void LifeTick(matrix<int>& m){
    matrix<int> newm(m.width(), m.hight());
    for (int y=0; y<matrix_h; y++){
        for (int x = 0; x<matrix_w; x++){
            if(m.get(x, y) == 0){
                if (CountAround(m, x, y)==3){
                    newm.set(x, y, 1);
                } else{
                    newm.set(x, y, 0);
                }
             } else {
                int a = CountAround(m, x, y);
                if (a==3 or a==2){
                    newm.set(x, y, 1);
                } else{
                    newm.set(x, y, 0);
                }
            }
        }
    }
    m = newm;
}


int main()
{
    matrix<int> m(matrix_w, matrix_h, 0);

    for (int y=0; y<matrix_h; y++){
        for (int x = 0; x<matrix_w; x++){
            m.get(x, y) = rand()%2;
        }
    }


    sf::RenderWindow window(sf::VideoMode(w, h), "pixel");
    window.setFramerateLimit(fps);
    sf::RenderTexture emptyTexture;

    emptyTexture.create(w, h);
    sf::Sprite grid = sf::Sprite(emptyTexture.getTexture());
    sf::Shader gridshader;
    gridshader.loadFromFile("/home/supsun/Documents/pixel/shaders/grid.frag", sf::Shader::Fragment);

    sf::Shader drawshader;
    drawshader.loadFromFile("/home/supsun/Documents/pixel/shaders/draw.frag", sf::Shader::Fragment);

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
        window.clear();

        DarwMatrix(m, 0, matrix_w, 0, matrix_h, window, drawshader);


        if (border_w>0){
            gridshader.setUniform("w", (float)w);
            gridshader.setUniform("h", (float)h);
            gridshader.setUniform("b_w", (float)border_w);
            gridshader.setUniform("b_c", (float)border_count);
            window.draw(grid, &gridshader);
        }

        window.display();

        LifeTick(m);

    }
    return 0;
}
