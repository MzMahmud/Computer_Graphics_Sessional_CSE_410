#include <bits/stdc++.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid ;
int drawaxes;
double angle;

#define DEGtoRED(x) (x*pi/180)
struct point{
    double x,y,z;
    point(double _x = .0,double _y = .0,double _z = .0){
        x = _x;y = _y;z = _z;
    }
    double mod(){return sqrt(x*x + y*y + z*z);}

    point operator+(const point& other) const {
        return point(this->x + other.x,this->y + other.y,this->z + other.z);
    }
    point operator-(const point& other) const {
        return point(this->x - other.x,this->y - other.y,this->z - other.z);
    }
    point operator-() const {
        return point(-this->x,-this->y,-this->z);
    }
    point operator*(double a) const {
        return point(this->x*a,this->y*a,this->z*a);
    }
    point operator/(double a) const {
        return point(this->x/a,this->y/a,this->z/a);
    }

    double dot(const point& other)const{
        return this->x*other.x
             + this->y*other.y
             + this->z*other.z;
    }
    point cross(const point& other)const{
        return point(this->y*other.z - this->z*other.y,
                     this->z*other.x - this->x*other.z,
                     this->x*other.y - this->y*other.x
                     );
    }
    point normalize(){
        return *this/this->mod();
    }
    //rotate this 3d vector (l) counterclockwise with respect to a 3d unit
    //vector r by an angle A, where l and r are perpendicular to each other
    point rotate(double A,const point& r){
        A = DEGtoRED(A);
        point l = *this;
        point u = r.cross(l);
        return u*sin(A) + l*cos(A);
    }
};

ostream& operator<<(ostream& sout,const point& p){
    sout << "(" << p.x << "," << p.y << "," << p.z << ")";
    return sout;
}
istream& operator>>(istream& sin,point& p){
    sin >> p.x >> p.y >> p.z;
    return sin;
}

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


point pos,u,r,l;
double width,radius;
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*pi/2);
			points[i][j].y=r*sin(((double)j/(double)slices)*pi/2);
			points[i][j].z=h;
		}
	}
	glColor3f(1.0,0,0);
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*pi/2);
        points[i].y=radius*sin(((double)i/(double)segments)*pi/2);
    }
    glColor3f(0,1.0,0);
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        //glColor3f(shade,shade,shade);

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,height);
			glVertex3f(points[i].x,points[i].y,height);
        }
        glEnd();
    }
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
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
        case '+':
			radius += 2;
			break;
		case '-':
			radius -= 2;
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			move_backward(2);
			break;
		case GLUT_KEY_UP:		// up arrow key
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
            radius += .2;
            if(radius >= width)
                radius = width;
			break;
		case GLUT_KEY_END:
		    radius -= .2;
            if(radius <= .2)
                radius = .0;
			break;

		default:
			break;
	}
}

void drawCapsule(double w,double r){
    glPushMatrix();
        glTranslatef(w-r,w-r,w-r);
        drawSphere(r,20,20);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(w-r,w-r,-(w-r));
        glRotatef(90, 0,1,0);
        drawSphere(r,20,20);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(w-r,w-r,-(w-r));
        drawCylinder(r,2*(w-r),20);
    glPopMatrix();
}

void drawEdge(double w,double r){
    glPushMatrix();
        glRotatef(-90, 0,1,0);
        glTranslatef(w-r,w-r,-(w-r));
        drawCylinder(r,2*(w-r),20);
    glPopMatrix();
}

void drawPlane(double a){
    a = abs(a);
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
        glVertex3f( a, a,0);
        glVertex3f(-a, a,0);
        glVertex3f(-a,-a,0);
        glVertex3f( a,-a,0);
    glEnd();
}

void drawBox(double w,double r){
    drawCapsule(w,r);

    glPushMatrix();
        glRotatef(90, 0,0,1);
        drawCapsule(w,r);
    glPopMatrix();

    glPushMatrix();
        glRotatef(180, 0,0,1);
        drawCapsule(w,r);
    glPopMatrix();

    glPushMatrix();
        glRotatef(-90, 0,0,1);
        drawCapsule(w,r);
    glPopMatrix();

    drawEdge(w,r);
    glPushMatrix();
        glRotatef(90, 1,0,0);
        drawEdge(w,r);
    glPopMatrix();

    glPushMatrix();
        glRotatef(180, 1,0,0);
        drawEdge(w,r);
    glPopMatrix();

    glPushMatrix();
        glRotatef(-90, 1,0,0);
        drawEdge(w,r);
    glPopMatrix();

    glPushMatrix();
        glRotatef(90, 0,0,1);
        drawEdge(w,r);
    glPopMatrix();

    glPushMatrix();
        glRotatef(90, 0,1,0);
        glRotatef(90, 0,0,1);
        drawEdge(w,r);
    glPopMatrix();

    glPushMatrix();
        glRotatef(180, 0,1,0);
        glRotatef(90, 0,0,1);
        drawEdge(w,r);
    glPopMatrix();

    glPushMatrix();
        glRotatef(-90, 0,1,0);
        glRotatef(90, 0,0,1);
        drawEdge(w,r);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0,w);
        drawPlane(w-r);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0,-w);
        drawPlane(w-r);
    glPopMatrix();


    glColor3f(1,0,0);
    glPushMatrix();
        glTranslatef(w,0,0);
        glRotatef(90, 0,1,0);
        drawPlane(w-r);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-w,0,0);
        glRotatef(90, 0,1,0);
        drawPlane(w-r);
    glPopMatrix();


	glColor3f(0,1,0);
    glPushMatrix();
        glTranslatef(0,w,0);
        glRotatef(90, 1,0,0);
        drawPlane(w-r);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,-w,0);
        glRotatef(90, 1,0,0);
        drawPlane(w-r);
    glPopMatrix();
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
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

void display(){
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();
    gluLookAt(pos.x,pos.y,pos.z,  pos.x + l.x,pos.y + l.y,pos.z + l.z, u.x,u.y,u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);
	drawAxes();
	drawGrid();
    drawBox(width,radius);
	glutSwapBuffers();
}


void move_up(double step){
    pos = pos + u*step;
}
void move_down(double step){
    pos = pos - u*step;
}
void move_forward(double step){
    pos = pos + l*step;
}
void move_backward(double step){
    pos = pos - l*step;
}
void move_left(double step){
    pos = pos - r*step;
}
void move_right(double step){
    pos = pos + r*step;
}

void look_left(double angle){
    l = l.rotate(angle,u);
    r = l.cross(u);
}
void look_right(double angle){
    l = l.rotate(-angle,u);
    r = l.cross(u);
}
void look_up(double angle){
    l = l.rotate(angle,r);
    u = r.cross(l);
}
void look_down(double angle){
    l = l.rotate(-angle,r);
    u = r.cross(l);
}
void tilt(double angle){
    u = u.rotate(angle,l);
    r = l.cross(u);
}

void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

	l   = point(-1/sqrt(2),-1/sqrt(2),0);
	r   = point(-1/sqrt(2), 1/sqrt(2),0);
	u   = point(0,0,1);
	pos = point(100,100,0);
	width  = 35;
	radius = 15;
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Sphere To Cube With Fully Rotatable Camera");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
