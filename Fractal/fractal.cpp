#include <GL/glut.h> // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>

using namespace std;
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define pi (acos(-1.0))
#define DEGtoRED(x) (x * pi / 180)
struct Vector3 {
    double x, y, z;
    Vector3(double _x = .0, double _y = .0, double _z = .0) {
        x = _x;
        y = _y;
        z = _z;
    }
    double mod() { return sqrt(x * x + y * y + z * z); }

    Vector3 operator+(const Vector3 &other) const {
        return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
    }
    Vector3 operator-(const Vector3 &other) const {
        return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
    }
    Vector3 operator-() const { return Vector3(-this->x, -this->y, -this->z); }
    Vector3 operator*(double a) const {
        return Vector3(this->x * a, this->y * a, this->z * a);
    }
    Vector3 operator/(double a) const {
        return Vector3(this->x / a, this->y / a, this->z / a);
    }

    double dot(const Vector3 &other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }
    Vector3 cross(const Vector3 &other) const {
        return Vector3(this->y * other.z - this->z * other.y,
                       this->z * other.x - this->x * other.z,
                       this->x * other.y - this->y * other.x);
    }
    Vector3 normalize() { return *this / this->mod(); }
    // rotate this 3d vector (l) counterclockwise with respect to a 3d unit
    // vector r by an angle A, where l and r are perpendicular to each other
    Vector3 rotate(double A, const Vector3 &r) {
        A         = DEGtoRED(A);
        Vector3 l = *this;
        Vector3 u = r.cross(l);
        return u * sin(A) + l * cos(A);
    }
};

ostream &operator<<(ostream &sout, const Vector3 &p) {
    sout << "(" << p.x << "," << p.y << "," << p.z << ")";
    return sout;
}
istream &operator>>(istream &sin, Vector3 &p) {
    sin >> p.x >> p.y >> p.z;
    return sin;
}

void drawKoch(Vector3 &pos, Vector3 &dir, double len, int gen) {
    if (!gen) {
        glBegin(GL_LINES);
        {
            glVertex3d(pos.x, pos.y, pos.z);
            pos = pos + dir * len;
            glVertex3d(pos.x, pos.y, pos.z);
        }
        glEnd();
        return;
    }
    drawKoch(pos, dir, len / 3.0, gen - 1);

    dir = dir.rotate(60, Vector3(0, 0, 1));
    drawKoch(pos, dir, len / 3.0, gen - 1);

    dir = dir.rotate(-120, Vector3(0, 0, 1));
    drawKoch(pos, dir, len / 3.0, gen - 1);

    dir = dir.rotate(60, Vector3(0, 0, 1));
    drawKoch(pos, dir, len / 3.0, gen - 1);
}

void drawKochSnow(Vector3 &pos, int gen) {
    Vector3 dir(1, 0, 0);

    dir = dir.rotate(60, Vector3(0, 0, 1));
    drawKoch(pos, dir, 1, gen);
    dir = dir.rotate(-120, Vector3(0, 0, 1));
    drawKoch(pos, dir, 1, gen);
    dir = dir.rotate(-120, Vector3(0, 0, 1));
    drawKoch(pos, dir, 1, gen);
}

/* Handler for window re-size event. Called back when the window first appears
   and whenever the window is re-sized with its new width and height */
void reshape(GLsizei width,
             GLsizei height) { // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix
    if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}

int gen = -3;
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case '0':
        gen = -3;
    }
}

int frameRate = 2;

void update(int value) {
    glutPostRedisplay(); // Inform GLUT that the display has changed
    glutTimerFunc(1000.0 / frameRate, update,
                  0); // Call update after each 25 millisecond
}

/* Initialize OpenGL Graphics */
void setup() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.f, 0.0f); // Black and opaque
}

void drawTree(Vector3 &pos, Vector3 dir, double len, int depth) {
    if (!depth)
        return;

    glBegin(GL_LINES);
    {
        glVertex3d(pos.x, pos.y, pos.z);
        pos = pos + dir * len;
        glVertex3d(pos.x, pos.y, pos.z);
    }
    glEnd();

    Vector3 prevPos = pos;
    drawTree(pos, dir.rotate(20, Vector3(0, 0, 1)), len * 3 / 4, depth - 1);
    drawTree(prevPos, dir.rotate(-20, Vector3(0, 0, 1)), len * 3.0 / 4,
             depth - 1);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vector3 pos(0, -1, 0);
    Vector3 dir(0, 1, 0);

    glColor3f(1, 1, 1);

    // drawKoch(pos, dir, 2, MAX(gen++, 0) % 10);

    // pos = Vector3(-.5, -.2, 0);
    // drawKochSnow(pos, MAX(gen++, 0) % 10);

    drawTree(pos, dir, .5, MAX(gen++, 0) % 20);

    glutSwapBuffers(); // Send the 3D scene to the screen
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Fractal");

    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(draw);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(reshape);
    glutTimerFunc(100, update, 0);
    setup();
    glutMainLoop();
    return 0;
}
