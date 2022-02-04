#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cmath>
#include <fstream>

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
        for(int i = 0; i<y; i++){
            a[i]= std::vector<T>(x);
            for (int j = 0; j < x; j++){
                a[i][j] = base;
            }
        }
    }
    T& get(int x, int y){
        if (this->borderCross){
            float fx = float(x);
            float fy = float(y);
            float fw = float(this->w);
            float fh = float(this->h);
            x  = fx - floor(fx/fw)*fw;
            y  = fy - floor(fy/fh)*fh;
        }
        return this->a[y][x];
    }
    void set(int x, int y, T newT){
        if (this->borderCross){
            float fx = float(x);
            float fy = float(y);
            float fw = float(this->w);
            float fh = float(this->h);
            x  = fx - floor(fx/fw)*fw;
            y  = fy - floor(fy/fh)*fh;
        }
        this->a[y][x] =  newT;
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

    friend std::ostream& operator<< (std::ostream &out, const matrix &mat);
    friend std::istream& operator>> (std::istream &in, matrix &mat);


private:
    int w;
    int h;
    bool borderCross;
    std::vector<std::vector<T>> a;


};

std::ostream& operator<< (std::ostream &out, const matrix<int> &mat){
    for(int y=0; y < mat.h; y++){
        for (int x = 0; x< mat.w; x++){
            out << mat.a[y][x] << " ";
        }
    }
    return out;
}

std::istream& operator>> (std::istream &in, matrix<int> &mat){
    for(int y=0; y < mat.h; y++){
        for (int x = 0; x< mat.w; x++){
            in >> mat.a[y][x];
        }
    }
    return in;
}
#endif // MATRIX_H
