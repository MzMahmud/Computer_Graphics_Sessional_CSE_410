#include "Object.cpp"
#include "bitmap_image.hpp"

using namespace std;

void move_up(double step);
void move_down(double step);
void move_forward(double step);
void move_backward(double step);
void move_right(double step);
void move_left(double step);

void look_left(double step);
void look_right(double step);

void look_up(double step);
void look_down(double step);

void tilt(double angle);

Vector3 pos, u, r, l;

int imageWidth = 768, imageHeight = 768;
int windowWidth = 500, windowHeight = 500;
int recursion_level = 3;
double viewAngleDeg = 90;

vector<Object*> objects;
vector<Vector3> lights;

void getInput() {
    ifstream fin("description.txt");

    fin >> recursion_level;
    cout << "level of recursion : " << recursion_level << endl;

    fin >> imageWidth;
    cout << "pixels : " << imageWidth << endl;
    imageHeight = imageWidth;

    int n_obj;
    fin >> n_obj;
    cout << "#objects : " << n_obj << endl;

    // Floor
    Object* floor = new Floor(600, 30);
    floor->setCoeff(0.2, 0.2, 0.2, 0.6);
    floor->setExponent(10);
    objects.push_back(floor);

    for (int i = 0; i < n_obj; i++) {
        string name;
        fin >> name;

        while (name[(int)name.length() - 1] == '\r')
            name.pop_back();

        if (name == string("sphere")) {
            double c[3];
            fin >> c[0] >> c[1] >> c[2];

            double r;
            fin >> r;

            Object* temp = new Sphere(c[0], c[1], c[2], r);

            double col[3];
            fin >> col[0] >> col[1] >> col[2];
            temp->setColor(col[0], col[1], col[2]);

            double coeff[4];
            for (int j = 0; j < 4; j++)
                fin >> coeff[j];
            temp->setCoeff(coeff[0], coeff[1], coeff[2], coeff[3]);

            double k;
            fin >> k;
            temp->setExponent(k);

            objects.push_back(temp);

        } else if (name == string("pyramid")) {

            double c[3];
            fin >> c[0] >> c[1] >> c[2];

            double l, h;
            fin >> l >> h;

            Vector3 v1 = Vector3(c[0], c[1], c[2]);
            Vector3 v2 = Vector3(c[0] + l, c[1], c[2]);
            Vector3 v3 = Vector3(c[0] + l, c[1] + l, c[2]);
            Vector3 v4 = Vector3(c[0], c[1] + l, c[2]);

            Vector3 up =
                Vector3((2 * c[0] + l) / 2, (2 * c[1] + l) / 2, c[2] + h);

            Object* base1 = new Triangle(v1, v2, v4);
            Object* base2 = new Triangle(v2, v3, v4);

            Object* triangle1 = new Triangle(v1, v2, up);
            Object* triangle2 = new Triangle(v2, v3, up);
            Object* triangle3 = new Triangle(v3, v4, up);
            Object* triangle4 = new Triangle(v4, v1, up);

            double col[3];
            fin >> col[0] >> col[1] >> col[2];

            base1->setColor(col[0], col[1], col[2]);
            base2->setColor(col[0], col[1], col[2]);

            triangle1->setColor(col[0], col[1], col[2]);
            triangle2->setColor(col[0], col[1], col[2]);
            triangle3->setColor(col[0], col[1], col[2]);
            triangle4->setColor(col[0], col[1], col[2]);

            double coeff[4];
            for (int j = 0; j < 4; j++)
                fin >> coeff[j];

            base1->setCoeff(coeff[0], coeff[1], coeff[2], coeff[3]);
            base2->setCoeff(coeff[0], coeff[1], coeff[2], coeff[3]);

            triangle1->setCoeff(coeff[0], coeff[1], coeff[2], coeff[3]);
            triangle2->setCoeff(coeff[0], coeff[1], coeff[2], coeff[3]);
            triangle3->setCoeff(coeff[0], coeff[1], coeff[2], coeff[3]);
            triangle4->setCoeff(coeff[0], coeff[1], coeff[2], coeff[3]);

            double k;
            fin >> k;
            base1->setExponent(k);
            base2->setExponent(k);
            triangle1->setExponent(k);
            triangle2->setExponent(k);
            triangle3->setExponent(k);
            triangle4->setExponent(k);

            // TODO: pyramid not implemented yet!
            objects.push_back(base1);
            objects.push_back(base2);
            objects.push_back(triangle1);
            objects.push_back(triangle2);
            objects.push_back(triangle3);
            objects.push_back(triangle4);
        }
    }

    int n_light;
    fin >> n_light;
    cout << "#lights : " << n_light << endl;

    for (int i = 0; i < n_light; i++) {
        double x, y, z;
        fin >> x >> y >> z;
        lights.push_back(Vector3(x, y, z));
    }

    fin.close();
}

pair<int, double> getNearest_index_t(const Ray& ray,
                                     const vector<Object*>& objects) {
    int min_index = -1;
    double min_t  = INT32_MAX;
    for (int i = 0; i < objects.size(); i++) {
        double t = objects[i]->t_intersection(ray);
        if (t > .0 && t < min_t) {
            min_t     = t;
            min_index = i;
        }
    }
    return make_pair(min_index, min_t);
}

void capture() {

    double viewAngle     = DEGtoRAD(viewAngleDeg);
    double planeDistance = (windowHeight / 2) / tan(viewAngle / 2);

    Vector3 topLeft = pos + (l * planeDistance - r * (windowWidth / 2) +
                             u * (windowHeight / 2));

    double du = (windowWidth * 1.0) / imageWidth;
    double dv = (windowHeight * 1.0) / imageHeight;

    bitmap_image image(imageWidth, imageHeight);

    cout << ".....Capturing Image....." << endl;
    for (int i = 0; i < imageWidth; i++) {
        for (int j = 0; j < imageHeight; j++) {
            Vector3 dir = topLeft + r * i * du - u * j * dv;

            Ray ray(pos, dir - pos);

            pair<int, double> pair = getNearest_index_t(ray, objects);
            int near_index = pair.first;
            double near_t  = pair.second;
            ColorRGB pixelColor(0, 0, 0);
            if (near_index != -1) {
                pixelColor = objects[near_index]->getColor(
                    ray, near_t, recursion_level, lights, objects);
            }
            image.set_pixel(i, j, pixelColor.r * 255, pixelColor.g * 255,
                            pixelColor.b * 255);
        }
    }
    image.save_image("out.bmp");
    cout << "*****Image CAPTURED!*****" << endl;
}

void keyboardListener(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        look_left(3);
        break;
    case '2':
        look_right(3);
        break;
    case '3':
        look_up(3);
        break;
    case '4':
        look_down(3);
        break;
    case '5':
        tilt(3);
        break;
    case '6':
        tilt(-3);
        break;
    case '0':
        capture();
        break;
    default:
        break;
    }
}

void specialKeyListener(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_DOWN: // down arrow key
        move_backward(2);
        break;
    case GLUT_KEY_UP: // up arrow key
        move_forward(2);
        break;

    case GLUT_KEY_RIGHT:
        move_right(2);
        break;
    case GLUT_KEY_LEFT:
        move_left(2);
        break;

    case GLUT_KEY_PAGE_UP:
        move_up(2);
        break;
    case GLUT_KEY_PAGE_DOWN:
        move_down(2);
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}

void mouseListener(int button, int state, int x,
                   int y) { // x, y is the x-y of the screen (2D)
    switch (button) {
    case GLUT_LEFT_BUTTON:

        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}

void display() {
    // clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0); // color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    // initialize the matrix
    glLoadIdentity();
    gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x,
              u.y, u.z);

    // again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);

    for (int i = 0; i < objects.size(); i++) {
        objects[i]->draw();
    }

    glutSwapBuffers();
}

void move_up(double step) { pos = pos + u * step; }
void move_down(double step) { pos = pos - u * step; }
void move_forward(double step) { pos = pos + l * step; }
void move_backward(double step) { pos = pos - l * step; }
void move_left(double step) { pos = pos - r * step; }
void move_right(double step) { pos = pos + r * step; }

void look_left(double angle) {
    l = l.rotate(angle, u);
    r = l.cross(u);
}
void look_right(double angle) {
    l = l.rotate(-angle, u);
    r = l.cross(u);
}
void look_up(double angle) {
    l = l.rotate(angle, r);
    u = r.cross(l);
}
void look_down(double angle) {
    l = l.rotate(-angle, r);
    u = r.cross(l);
}
void tilt(double angle) {
    u = u.rotate(angle, l);
    r = l.cross(u);
}

void animate() {
    // codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init() {
    // codes for initialization

    // clear the screen
    glClearColor(0, 0, 0, 0);

    // load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    // initialize the matrix
    glLoadIdentity();

    // give PERSPECTIVE parameters
    gluPerspective(viewAngleDeg, 1, 1, 1000.0);
    // field of view in the Y (vertically)
    // aspect ratio that determines the field of view in the X direction
    // (horizontally)
    // near distance
    // far distance

    pos = Vector3(100, 100, 0);
    l   = Vector3(-1 / sqrt(2), -1 / sqrt(2), 0);
    r   = Vector3(-1 / sqrt(2), 1 / sqrt(2), 0);
    u   = Vector3(0, 0, 1);

    move_up(50);
    look_down(20);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(windowHeight, windowWidth);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE |
                        GLUT_RGB); // Depth, Double buffer, RGB color

    glutCreateWindow("Ray Tracing");

    init();

    glEnable(GL_DEPTH_TEST); // enable Depth Testing

    glutDisplayFunc(display); // display callback function
    glutIdleFunc(animate);    // what you want to do in the idle time (when no
                              // drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    getInput();
    glutMainLoop(); // The main loop of OpenGL

    return 0;
}
