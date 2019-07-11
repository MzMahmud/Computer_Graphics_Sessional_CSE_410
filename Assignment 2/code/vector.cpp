//--------Vector Class--------
#include "vector.h"

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
