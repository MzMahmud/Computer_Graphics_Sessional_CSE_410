#include <GL/glut.h>
#include <bits/stdc++.h>

#define PI (acos(-1.0))
#define DEGtoRAD(x) ((x)*PI / 180)

using namespace std;

class Vector3 {
  public:
    double x, y, z;
    Vector3(double _x = .0, double _y = .0, double _z = .0)
        : x(_x), y(_y), z(_z) {}

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

#define IN_RANGE(x, a, b) ((a) <= (x) && (x) <= (b))

class Ray;
class Object;

pair<int, double> getNearest_index_t(const Ray& ray,
                                     const vector<Object*>& objects);

class Ray {
  public:
    Vector3 start, dir;
    Ray(Vector3 _start, Vector3 _dir) : start(_start), dir(_dir.normalize()) {}
};

double clippedValue(double val, double min, double max) {
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

struct ColorRGB {
    double r, g, b;

    ColorRGB() {}
    ColorRGB(double _r, double _g, double _b) : r(_r), g(_g), b(_b) {}

    ColorRGB& operator*=(double val) {
        r *= val;
        g *= val;
        b *= val;
        return *this;
    }

    ColorRGB& operator+=(const ColorRGB& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }
    ColorRGB& operator+=(double val) {
        *this += ColorRGB(val * r, val * g, val * b);
        return *this;
    }
    ColorRGB& clip() {
        r = clippedValue(r, 0, 1);
        g = clippedValue(g, 0, 1);
        b = clippedValue(b, 0, 1);
        return *this;
    }
};

ColorRGB operator*(const ColorRGB& color, double val) {
    ColorRGB temp = color;
    return temp *= val;
}
ColorRGB operator*(double val, const ColorRGB& color) {
    ColorRGB temp = color;
    return temp *= val;
}
ColorRGB operator+(const ColorRGB& a, const ColorRGB& b) {
    ColorRGB temp = a;
    return temp += b;
}

Vector3 reflectedRayDirection(const Ray& ray, Vector3 normal) {
    Vector3 ref = ray.dir - normal * 2.0 * ray.dir.dot(normal);
    return ref.normalize();
}

class Object {
  public:
    ColorRGB color;

    double k_a; // ambient coefficient
    double k_d; // diffuse coefficient
    double k_s; // specular coefficient
    double k_r; // reflection coefficient
    double k;   // specular exponent

    virtual ~Object() {}

    void setColor(double r, double g, double b) { color = ColorRGB(r, g, b); }

    void setCoeff(double ka, double kd, double ks, double kr) {
        k_a = ka;
        k_d = kd;
        k_s = ks;
        k_r = kr;
    }

    void setExponent(double _k) { k = _k; }

    ColorRGB getColor(const Ray& ray, double t, int level,
                      const vector<Vector3>& lights,
                      const vector<Object*>& objects) {

        ColorRGB colorOut(0, 0, 0);
        Vector3 point      = ray.start + ray.dir * t;
        Vector3 normal     = getNormal(point);
        Vector3 reflection = reflectedRayDirection(ray, normal);

        for (int i = 0; i < lights.size(); i++) {
            double ambientComp = .0, diffuseComp = .0, specularComp = .0;

            Vector3 LP_dir = lights[i] - point;
            Vector3 start  = point + LP_dir;

            Ray L(start, LP_dir);
            Vector3 R = reflectedRayDirection(L, normal).normalize();
            Vector3 V = point.normalize();

            int near_index = getNearest_index_t(L, objects).first;
            if (objects[near_index] == this) {
                diffuseComp  = k_d * max(L.dir.dot(normal), .0);
                specularComp = k_s * max(pow(R.dot(V), k), .0);

                diffuseComp  = diffuseComp > 1.0 ? 1.0 : diffuseComp;
                specularComp = specularComp > 1.0 ? 1.0 : specularComp;
            }

            ambientComp = k_a;
            colorOut += (color * (ambientComp + diffuseComp + specularComp));

            if (level > 0) {
                Vector3 start = point + reflection;
                Ray reflectionRay(start, reflection);
                pair<int, double> near_index_t =
                    getNearest_index_t(reflectionRay, objects);
                int near_index = near_index_t.first;
                double near_t  = near_index_t.second;
                if (near_index >= 0) {
                    ColorRGB ref = objects[near_index]->getColor(
                        reflectionRay, near_t, level - 1, lights, objects);
                    colorOut += (ref * k_r);
                }
            }
        }
        return colorOut.clip();
    }

    virtual void draw()                             = 0;
    virtual double t_intersection(const Ray& ray)   = 0;
    virtual Vector3 getNormal(const Vector3& point) = 0;
};

class Sphere : public Object {
  public:
    Vector3 center;
    double radius;

    Sphere() {}
    Sphere(double cX, double cY, double cZ, double r)
        : center(Vector3(cX, cY, cZ)), radius(r) {}

    ~Sphere() {}

    void draw() override {
        glPushMatrix();

        glColor3d(color.r, color.g, color.b);
        glTranslated(center.x, center.y, center.z);
        glutSolidSphere(radius, 50, 50);

        glPopMatrix();
    }

    double t_intersection(const Ray& ray) override {
        Vector3 CR = ray.start - center;
        double b   = ray.dir.dot(CR);

        double D = b * b - CR.dot(CR) + radius * radius; // Discriminant

        if (D < 0)
            return -1;

        double t1 = -b + sqrt(D);
        double t2 = -b - sqrt(D);

        return min(t1, t2);
    }

    Vector3 getNormal(const Vector3& point) override {
        Vector3 normal = point - center;
        return normal.normalize();
    }
};

class Triangle : public Object {
  public:
    Vector3 a, b, c;

    Triangle() {}
    Triangle(Vector3 _1, Vector3 _2, Vector3 _3) : a(_1), b(_2), c(_3) {}

    ~Triangle() {}

    void draw() override {

        glBegin(GL_TRIANGLES);
        {
            glColor3d(color.r, color.g, color.b);
            glVertex3d(a.x, a.y, a.z);
            glVertex3d(b.x, b.y, b.z);
            glVertex3d(c.x, c.y, c.z);
        }
        glEnd();
    }

    double t_intersection(const Ray& ray) override {
        Vector3 n  = getNormal(a);
        Vector3 P0 = a;
        Vector3 R0 = ray.start;
        Vector3 Rd = ray.dir;

        if (Rd.dot(n) == .0)
            return -1.0;

        double t = (P0 - R0).dot(n) / Rd.dot(n);

        Vector3 p = R0 + Rd * t; // intersection point

        Vector3 ab = b - a; // AB edge
        Vector3 bc = c - b; // BC edge
        Vector3 ca = a - c; // CA edge

        Vector3 norm = ab.cross(p - b); // AB x BP
        if (norm.dot(n) < .0)
            return -1;

        norm = bc.cross(p - c); // BC x CP
        if (norm.dot(n) < .0)
            return -1;

        norm = ca.cross(p - a); // CA x AP
        if (norm.dot(n) < .0)
            return -1;

        return t;
    }

    Vector3 getNormal(const Vector3& point) override {
        Vector3 ab = b - a;
        Vector3 ac = c - a;
        return ab.cross(ac).normalize();
    }
};

class Floor : public Object {
  public:
    int n;
    Vector3 bottomLeft;
    double floorLen, tileLen;

    Floor(double _floorLen, double _tileLen)
        : floorLen(_floorLen), tileLen(_tileLen) {

        bottomLeft = Vector3(-floorLen / 2.0, -floorLen / 2.0, 0.0);
        n          = floorLen / tileLen;
    }

    void draw() override {
        glBegin(GL_QUADS);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if ((i + j) & 0x00000001)
                    glColor3d(1.0, 1.0, 1.0);
                else
                    glColor3d(0.0, 0.0, 0.0);
                glVertex3d(bottomLeft.x + tileLen * i,
                           bottomLeft.y + tileLen * j, bottomLeft.z);
                glVertex3d(bottomLeft.x + tileLen * (i + 1),
                           bottomLeft.y + tileLen * j, bottomLeft.z);
                glVertex3d(bottomLeft.x + tileLen * (i + 1),
                           bottomLeft.y + tileLen * (j + 1), bottomLeft.z);
                glVertex3d(bottomLeft.x + tileLen * i,
                           bottomLeft.y + tileLen * (j + 1), bottomLeft.z);
            }
        }
        glEnd();
    }

    double t_intersection(const Ray& ray) override {
        if (ray.dir.z == .0)
            return -1;
        double t      = -(ray.start.z / ray.dir.z);
        Vector3 point = ray.start + ray.dir * t;
        int i         = (point.x - bottomLeft.x) / tileLen;
        int j         = (point.y - bottomLeft.y) / tileLen;
        if (!IN_RANGE(i, 0, n - 1) || !IN_RANGE(j, 0, n - 1))
            return -1;
        double col = (i + j) & 0x00000001;
        setColor(col, col, col);
        return t;
    }

    Vector3 getNormal(const Vector3& point) override {
        return Vector3(0, 0, 1); // always z-axis
    }
};
