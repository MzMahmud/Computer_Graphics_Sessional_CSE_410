#include "Vector3.cpp"
#include <GL/glut.h>

class Ray {
  public:
    Vector3 start, dir;

    Ray(Vector3 _start, Vector3 _dir) {
        start = _start;
        dir   = _dir;
        dir.normalize();
    }
};

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

    Vector3 reflectedRayDirection(const Ray& ray, Vector3 normal) {
        Vector3 ref = ray.dir - normal * 2.0 * ray.dir.dot(normal);
        return ref.normalize();
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
    Sphere(double cX, double cY, double cZ, double r) {
        center = Vector3(cX, cY, cZ);
        radius = r;
    }

    ~Sphere() {}

    void draw() override {
        glPushMatrix();

        glColor3d(color.r, color.g, color.b);
        glTranslated(center.x, center.y, center.z);
        glutSolidSphere(radius, 50, 50);

        glPopMatrix();
    }

    double t_intersection(const Ray& ray) override {
        double a, b, c, d;

        a = ray.dir.dot(ray.dir);
        b = ray.dir.dot(ray.start) * 2;
        c = ray.start.dot(ray.start) - radius * radius;

        d = b * b - 4 * a * c;
        if (d < 0)
            return -1.0;

        double t1 = (-b + sqrt(d)) / (2 * a);
        double t2 = (-b - sqrt(d)) / (2 * a);

        return min(t1, t2);
    }

    Vector3 getNormal(const Vector3& point) override {
        Vector3 normal = point - center;
        return normal.normalize();
    }
};

class Triangle : public Object {
  public:
    Vector3 v1, v2, v3;

    Triangle() {}
    Triangle(Vector3 _1, Vector3 _2, Vector3 _3) : v1(_1), v2(_2), v3(_3) {}

    ~Triangle() {}

    void draw() override {

        glBegin(GL_TRIANGLES);
        {
            glColor3d(color.r, color.g, color.b);
            glVertex3d(v1.x, v1.y, v1.z);
            glVertex3d(v2.x, v2.y, v2.z);
            glVertex3d(v3.x, v3.y, v3.z);
        }
        glEnd();
    }
    double t_intersection(const Ray& ray) override { return -1; }
    Vector3 getNormal(const Vector3& point) override {
        return Vector3(1, 0, 0);
    }
};

class Square : public Object {
  public:
    Vector3 v;
    double len;

    Square() {}
    Square(Vector3 _1, double _len) : v(_1), len(_len) {}

    ~Square() {}

    void draw() override {
        glBegin(GL_QUADS);
        {
            glColor3d(color.r, color.g, color.b);
            glVertex3d(v.x, v.y, v.z);
            glVertex3d(v.x + len, v.y, v.z);
            glVertex3d(v.x + len, v.y + len, v.z);
            glVertex3d(v.x, v.y + len, v.z);
        }
        glEnd();
    }

    double t_intersection(const Ray& ray) override { return -1; }
    Vector3 getNormal(const Vector3& point) override {
        return Vector3(1, 0, 0);
    }
};

class Floor : public Object {

  public:
    Vector3 center;
    double floorLen, tileLen;
    int n;

    Floor(double _floorLen, double _tileLen) {
        floorLen = _floorLen;
        tileLen  = _tileLen;
        center   = Vector3(-floorLen / 2.0, -floorLen / 2.0, 0.0);
        n        = floorLen / tileLen;
    }

    void draw() override {
        glBegin(GL_QUADS);
        {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {

                    if ((i + j) & 0x00000001)
                        glColor3d(1.0, 1.0, 1.0);
                    else
                        glColor3d(0.0, 0.0, 0.0);

                    glVertex3d(center.x + tileLen * i, center.y + tileLen * j,
                               center.z);
                    glVertex3d(center.x + tileLen * (i + 1),
                               center.y + tileLen * j, center.z);
                    glVertex3d(center.x + tileLen * (i + 1),
                               center.y + tileLen * (j + 1), center.z);
                    glVertex3d(center.x + tileLen * i,
                               center.y + tileLen * (j + 1), center.z);
                }
            }
        }
        glEnd();
    }

    double t_intersection(const Ray& ray) override { return -1; }
    Vector3 getNormal(const Vector3& point) override {
        return Vector3(1, 0, 0);
    }
};
