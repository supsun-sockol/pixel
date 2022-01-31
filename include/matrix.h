#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cmath>

template <typename T>
class matrix
{
public:
    matrix(){
        this->w = 0;
        this->h = 0;
        this->a =  std::vector<std::vector<T>>(0);
        this->borderCross = true;
    }
    matrix(int x, int y){
        this->w=x;
        this->h=y;
        this->borderCross = true;
        this->a = std::vector<std::vector<T>>(x);
        for(int i= 0; i<y; i++){
            a[i]= std::vector<T>(y);
        }
    }
    matrix(int x, int y, T base){
        this->w=x;
        this->h=y;
        this->borderCross = true;
        this->a = std::vector<std::vector<T>>(y);
        for(int i; i<y; i++){
            a[i]= std::vector<T>(x);
            for (int j =0; j < x; j++){
                a[i][j] = base;
            }
        }
    }
    T& get(int x, int y){
        if (this->borderCross){
            float fx = float(x);
            float fy = float(y);
            float fw = float(w);
            x  = fx - floor(fx/fw)*fw;
            y  = fy - floor(fy/fw)*fw;
        }
        return this->a[x][y];
    }
    void set(int x, int y, T newT){
        if (this->borderCross){
            float fx = float(x);
            float fy = float(y);
            float fw = float(w);
            x  = fx - floor(fx/fw)*fw;
            y  = fy - floor(fy/fw)*fw;
        }
        this->a[x][y] =  newT;
    }
    void BorderOn(){
        this->borderCross = false;
    }
    void BorderOff() {
        this->borderCross = true;
    }
    int width(){
        return this->w;
    }
    int hight(){
        return this->h;
    }
    matrix<T>& operator=(const matrix<T> mat){
        if(this != &mat){
            this->w = mat.w;
            this->h = mat.h;
            this->borderCross = mat.borderCross;
            this->a = std::vector<std::vector<T>>(mat.a);
        }
        return *this;
    }


private:
    int w;
    int h;
    bool borderCross;
    std::vector<std::vector<T>> a;


};

#endif // MATRIX_H
