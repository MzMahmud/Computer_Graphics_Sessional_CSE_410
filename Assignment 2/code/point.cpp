#include "point.h"

//------point class------

point::point(){
    double arr[] = { 0.0,
                     0.0,
                     0.0,
                     1.0 };
    x = Matrix(4,1,arr);
}
point::point(double _x,double _y,double _z){
    double arr[] = { _x,
                     _y,
                     _z,
                    1.0 };
    x = Matrix(4,1,arr);
}
point::point(const Matrix& mat){
    x = mat;
}
point::point(const point& other){
    this->x = other.x;
}
point& point::operator=(const point& other){
    this->x = other.x;
    return *this;
}

void point::makeHomogeneous(){
    if(this->x(3,0) != 0)
        this->x /= this->x(3,0);
}

std::ostream& operator<<(std::ostream& sout, const point& p){
    Matrix mat = p.x;
    sout << mat(0,0) << " " << mat(1,0) << " " << mat(2,0);
    return sout;
}

point operator*(const Matrix& m, const point& p){
    point out = point(m*p.x);
    out.makeHomogeneous();
    return out;
}
