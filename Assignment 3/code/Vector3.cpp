#include <bits/stdc++.h>

#define PI (acos(-1.0))
#define DEGtoRAD(x) ((x)*PI / 180)

using namespace std;

class Vector3 {
  public:
    double x, y, z;
    Vector3(double _x = .0, double _y = .0, double _z = .0) {
        x = _x;
        y = _y;
        z = _z;
    }
    double mod() { return sqrt(x * x + y * y + z * z); }

    Vector3 operator+(const Vector3& other) const {
        return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
    }
    Vector3 operator-(const Vector3& other) const {
        return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
    }
    Vector3 operator-() const { return Vector3(-this->x, -this->y, -this->z); }
    Vector3 operator*(double a) const {
        return Vector3(this->x * a, this->y * a, this->z * a);
    }
    Vector3 operator/(double a) const {
        return Vector3(this->x / a, this->y / a, this->z / a);
    }

    double dot(const Vector3& other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }
    Vector3 cross(const Vector3& other) const {
        return Vector3(this->y * other.z - this->z * other.y,
                       this->z * other.x - this->x * other.z,
                       this->x * other.y - this->y * other.x);
    }
    Vector3 normalize() { return *this / this->mod(); }
    // rotate this 3d vector (l) counterclockwise with respect to a 3d unit
    // vector r by an angle A, where l and r are perpendicular to each other
    Vector3 rotate(double A, const Vector3& r) {
        A         = DEGtoRAD(A);
        Vector3 l = *this;
        Vector3 u = r.cross(l);
        return u * sin(A) + l * cos(A);
    }
};

std::ostream& operator<<(std::ostream& sout, const Vector3& p) {
    sout << "(" << p.x << "," << p.y << "," << p.z << ")";
    return sout;
}
std::istream& operator>>(std::istream& sin, Vector3& p) {
    sin >> p.x >> p.y >> p.z;
    return sin;
}
