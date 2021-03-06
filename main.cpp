#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "matrix.h"
#include <random>
#include <boost/program_options.hpp>
#include <cstdio>
namespace po = boost::program_options;

enum modes{
    newRandom,
    fromFile,
    graphicRandom
};
modes work_mode=newRandom;
int matrix_w = 50;
int matrix_h = 50;
int pixel_w = 50;
const int border_count = 10;
int fps = 5;
int count = matrix_h*matrix_w;
std::string filename="../../files/test_an.lpix";
int border_w = floor((float)pixel_w/(float)border_count);


float* GetShaderArray(matrix<int>& m, int min_x, int max_x, int min_y, int max_y){
    float* arr = new float[(max_x-min_x)*(max_y-min_y)];
    for (int y = min_y; y<max_y; y++){
        for (int x = min_x; x<max_x; x++){
            arr[y*(max_x-min_x)+x-min_x] = m.get(x, y);
        }
    }
    return arr;
}

void AnalizeFile(std::string tempFileName, std::string resultFileName){
    std::ifstream tmpf(tempFileName, std::ios::binary);

    std::string str, fin;
    getline(tmpf, str, '\n');
    getline(tmpf, str, '\n');
    int size = str.size();

    tmpf.seekg(-size-1, std::ios_base::end);
    getline(tmpf, fin, '\n');

    tmpf.seekg(size+1, std::ios_base::beg);
    while (str!=fin){
        getline(tmpf, str, '\n');
    }
    std::ios::pos_type spos = tmpf.tellg();
    getline(tmpf, fin, '\n');
    while (str!=fin && !tmpf.eof()){
        getline(tmpf, str, '\n');
    }
    std::ios::pos_type fpos = tmpf.tellg();
    int cycle_size = int(fpos)-int(spos)-1;
    if (tmpf.eof()){
        fin = str;
        fpos = spos;
        cycle_size = 0;
        tmpf.clear();
    }

    tmpf.seekg(0, std::ios_base::beg);
    std::ofstream newf(resultFileName, std::ios::binary);
    while (fpos!=tmpf.tellg()){
        getline(tmpf, str, '\n');
        newf << str << "\n";
    }
    newf << "#" << cycle_size;
    tmpf.close();
    newf.close();
    remove(tempFileName.c_str());


}

void DarwMatrix(matrix<int>& m, int min_x, int max_x, int min_y, int max_y, int w, int h, sf::RenderWindow& window, sf::Shader& drawshader){
    if((max_x-min_x)*(max_y-min_y)<=10000){
        sf::RenderTexture drawTexture;
        drawTexture.create(w, h);
        sf::Sprite draw = sf::Sprite(drawTexture.getTexture());
        drawshader.setUniform("w", (float)m.width());
        drawshader.setUniform("h", (float)m.hight());
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


int main(int argc, char* argv[])
{
   std::string  string_work_mode;
   po::options_description desc("Allowed options");
   desc.add_options()
              ("t", po::value<std::string>(&string_work_mode), "set work mode(REQUIRED)")
              ("h", po::value<int>(&matrix_h), "set hight (REQUIRED)")
              ("w", po::value<int>(&matrix_w), "set width (REQUIRED)")
              ("p", po::value<int>(&pixel_w), "")//->default_value(1), "")
              ("f", po::value<int>(&fps)->default_value(1), "")
              ("c", po::value<int>(&count), "")
              ("help", "produce help message");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (!vm.count("t")||!vm.count("h")||!vm.count("w")) {
        std::cout << "Usage: pixel [options]\n" << desc;
        exit(EXIT_FAILURE);
    }

    if (string_work_mode=="fromFile"){
        std::ifstream ifile(filename, std::ios::binary);
        ifile >> matrix_h;
        ifile >> matrix_w;
        int w = matrix_w*(pixel_w+border_w)+border_w;
        int h = matrix_h*(pixel_w+border_w)+border_w;
        matrix<int> m(matrix_w, matrix_h, 0);




        sf::RenderWindow window(sf::VideoMode(w, h), "pixel");
        window.setFramerateLimit(fps);
        sf::RenderTexture emptyTexture;

        emptyTexture.create(w, h);
        sf::Sprite grid = sf::Sprite(emptyTexture.getTexture());
        sf::Shader gridshader;
        gridshader.loadFromFile("../../shaders/grid.frag", sf::Shader::Fragment);

        sf::Shader drawshader;
        drawshader.loadFromFile("../../shaders/draw.frag", sf::Shader::Fragment);
        while (window.isOpen())
        {
            if (ifile.peek()!='#'){
                ifile >> m;
            } else {
                ifile.ignore('#');
                int cycle_size;
                ifile >> cycle_size;
                ifile.seekg(-cycle_size);
            }
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }



            }
            window.clear();

            DarwMatrix(m, 0, matrix_w, 0, matrix_h, w, h, window, drawshader);


            if (border_w>0){
                gridshader.setUniform("w", (float)w);
                gridshader.setUniform("h", (float)h);
                gridshader.setUniform("b_w", (float)border_w);
                gridshader.setUniform("b_c", (float)border_count);
                window.draw(grid, &gridshader);
            }

            window.display();

        }
    }
    /*else if (string_work_mode=="graphicRandom")
    {
        std::ofstream ofile(filename, std::ios::binary);
        srand(time(0));
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
        gridshader.loadFromFile("../shaders/grid.frag", sf::Shader::Fragment);

        sf::Shader drawshader;
        drawshader.loadFromFile("../shaders/draw.frag", sf::Shader::Fragment);

        while (window.isOpen())
        {
            ofile<<m<<"\n";
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }



            }
            window.clear();

            DarwMatrix(m, 0, matrix_w, 0, matrix_h, w, h, window, drawshader);


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
        ofile.close();

    }*/
    else if (string_work_mode=="newRandom"){
        std::string file = filename+std::string(".temp");
        std::ofstream tmpf(file,  std::ios::binary);
        srand(time(0));
        matrix<int> m(matrix_w, matrix_h, 0);

        for (int y=0; y<matrix_h; y++){
            for (int x = 0; x<matrix_w; x++){
                m.get(x, y) = rand()%2;
            }
        }
        tmpf << m.hight() << " " << m.width() << "\n";
        for (int i=0; i<count; i++){
            /*if(i%(count/100)==0){
                std::cout<<"|";
            }*/
            tmpf<<m<<"\n";
            LifeTick(m);
        }
        tmpf.close();
        AnalizeFile(file, filename);
    }

    return 0;
}
