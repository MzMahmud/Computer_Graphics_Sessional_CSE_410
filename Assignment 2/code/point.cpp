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


//--------Vector Class--------

Vector::Vector(){
    x = 0.0;y = 0.0;z = 0.0;
}

Vector::Vector(double _x,double _y,double _z){
    x = _x;y = _y;z = _z;
}

double Vector::mod(){
    return sqrt(x*x + y*y + z*z);
}

Vector Vector::operator+(const Vector& other) const {
    return Vector(this->x + other.x,this->y + other.y,this->z + other.z);
}

Vector Vector::operator-(const Vector& other) const {
    return Vector(this->x - other.x,this->y - other.y,this->z - other.z);
}

Vector Vector::operator-() const {
    return Vector(-this->x,-this->y,-this->z);
}

Vector Vector::operator*(double a) const {
    return Vector(this->x*a,this->y*a,this->z*a);
}

Vector Vector::operator/(double a) const {
    return Vector(this->x/a,this->y/a,this->z/a);
}

double Vector::dot(const Vector& other)const{
    return this->x*other.x
         + this->y*other.y
         + this->z*other.z;
}
Vector Vector::cross(const Vector& other)const{
    return Vector(this->y*other.z - this->z*other.y,
                 this->z*other.x - this->x*other.z,
                 this->x*other.y - this->y*other.x
                 );
}
Vector Vector::normalize(){
    return *this/this->mod();
}
