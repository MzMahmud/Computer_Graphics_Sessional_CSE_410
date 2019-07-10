#ifndef __POINT_H__
#define __POINT_H__

#include "matrix.h"
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


class point{
public:
    Matrix x;

    point();
    point(double _x,double _y,double _z);
    point(const Matrix& mat);
    point(const point& other);

    void makeHomogeneous();
    point& operator=(const point& other);
};


std::ostream& operator<<(std::ostream& sout, const point& p);

point operator*(const Matrix& m, const point& p);

#endif
