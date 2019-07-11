#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>

#define DEGtoRED(x) ( (x)*acos(-1.0)/180.0 )

class Vector{
public:
    double x,y,z;

    Vector();
    Vector(double _x,double _y,double _z);
    double mod();

    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator-() const;
    Vector operator*(double a) const;
    Vector operator/(double a) const;

    double dot(const Vector& other)const;
    Vector cross(const Vector& other)const;

    Vector normalize();
};

#endif
