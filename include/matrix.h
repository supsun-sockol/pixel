#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

template <typename T>
class matrix
{
public:
    matrix(){
        this->x = 0;
        this->y = 0;
        this->a =  std::vector<std::vector<T>>(0);
    }
    matrix(int x, int y){
        this->x=x;
        this->y=y;
        this->a = std::vector<std::vector<T>>(y);
        for(int i; i<y; i++){
            a[i]= std::vector<T>(x);
        }
    }
    T get(int x, int y){
        return a[x%this->x][y%this->y];
    }
    void set(int x, int y, T newT);
private:
    int w;
    int h;
    bool borderCross;
    std::vector<std::vector<T>> a;


};

#endif // MATRIX_H
