#include <bits/stdc++.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
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
    //vector r by an angle A(in degree), where l and r are perpendicular to each other
    point rotate(double A,const point& r){
        A = DEGtoRED(A);
        point l = *this;
        point u = r.cross(l);
        return u*sin(A) + l*cos(A);
    }
};

std::ostream& operator<<(std::ostream& sout,const point& p){
    sout << "(" << p.x << "," << p.y << "," << p.z << ")";
    return sout;
}

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
		glColor3f(0.4, 0.4, 0.4);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				//if(i==0)
				//	continue;	//SKIP the MAIN axes

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



point pos,dir;

void move(double);
void rotate(double);

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
        case 'w':
			move( 1);
			break;
        case 's':
			move(-1);
			break;
        case 'a':
			rotate( 2);
			break;
        case 'd':
			rotate(-2);
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
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

double wheelX,wheelA,wheelRad,wheelDir;

void drawWheel(double radius,double width){
    int segments = 32;
    int i;
    double shade;
    struct point points[100];
    //generate points
    glPushMatrix();
    glTranslatef(pos.x,pos.y,pos.z + wheelRad);
    glRotatef(atan2(-dir.y,-dir.x)/pi*180, 0,0,1);
    glRotatef(90, 1,0,0);
    glRotatef(wheelA, 0,0,1);

    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*pi*2);
        points[i].y=radius*sin(((double)i/(double)segments)*pi*2);
    }
    //glColor3f(0,1.0,0);
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,0,shade*.1);

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x  ,points[i].y  ,-width/2);
			glVertex3f(points[i+1].x,points[i+1].y,-width/2);
			glVertex3f(points[i+1].x,points[i+1].y, width/2);
			glVertex3f(points[i].x  ,points[i].y  , width/2);
        }
        glEnd();
    }
    glBegin(GL_QUADS);
    {
        glColor3f(.6,.6*.1,.6*.1);
        glVertex3f(radius,0,-width/4);
        glVertex3f(-radius,0,-width/4);
        glVertex3f(-radius,0, width/4);
        glVertex3f(radius,0, width/4);

        glVertex3f(0,radius,-width/4);
        glVertex3f(0,-radius,-width/4);
        glVertex3f(0,-radius, width/4);
        glVertex3f(0, radius, width/4);
    }
    glEnd();
    glPopMatrix();
}

void move(double step){
    pos = pos + dir*step;
    double dA = 360.0/(2*pi*wheelRad)*step;
    wheelA += dA;
    if(wheelA > 360)
        wheelA -= 360;
}

void rotate(double angleDeg){//
    dir = dir.rotate(angleDeg, point(0,0,1) );
    //std::cout << dir << std::endl;
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(80*cos(cameraAngle), 80*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glRotatef(wheelDir, 0,0,1);

	drawWheel(wheelRad,wheelRad/3);
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=0;
	cameraHeight=60.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(70,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

	wheelRad = 12;
	wheelX = wheelDir = wheelA = 0;
	pos = point( 0, 0, 0);
    dir = point(-1, 0, 0);
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

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
