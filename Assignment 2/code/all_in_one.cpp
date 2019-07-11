#include <bits/stdc++.h>

#define DEGtoRED(x) ( (x)*acos(-1.0)/180.0 )

using namespace std;

// ------matrix.h------
class Matrix {
    public:
        Matrix(int, int);
        Matrix(int ,int ,double *);
        Matrix();
        ~Matrix();
        Matrix(const Matrix&);
        Matrix& operator=(const Matrix&);

        inline double& operator()(int x, int y) { return p[x][y]; }

        Matrix& operator*=(const Matrix&);
        Matrix& operator/=(double);

        static Matrix createIdentity(int);
        static Matrix createZero(int);

    private:
        int rows_, cols_;
        double **p;

        void allocSpace();
};

Matrix operator*(const Matrix&, const Matrix&);
// ------matrix.h END------

// ------point.h------
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
//------point.h END------

//------vector.h------
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
//------vector.h END------

//------matrix.cpp------
Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols)
{
    allocSpace();
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            p[i][j] = 0;
        }
    }
}

Matrix::Matrix(int rows, int cols,double* arr) : rows_(rows), cols_(cols)
{
    allocSpace();
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            int index = i*cols_ + j;
            p[i][j] = arr[index];
        }
    }
}

Matrix::Matrix() : rows_(1), cols_(1)
{
    allocSpace();
    p[0][0] = 0;
}

Matrix::~Matrix()
{
    for (int i = 0; i < rows_; ++i) {
        delete[] p[i];
    }
    delete[] p;
}

Matrix::Matrix(const Matrix& m) : rows_(m.rows_), cols_(m.cols_)
{
    allocSpace();
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            p[i][j] = m.p[i][j];
        }
    }
}

Matrix& Matrix::operator=(const Matrix& m)
{
    if (this == &m) {
        return *this;
    }

    if (rows_ != m.rows_ || cols_ != m.cols_) {
        for (int i = 0; i < rows_; ++i) {
            delete[] p[i];
        }
        delete[] p;

        rows_ = m.rows_;
        cols_ = m.cols_;
        allocSpace();
    }

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            p[i][j] = m.p[i][j];
        }
    }
    return *this;
}


Matrix& Matrix::operator*=(const Matrix& m)
{
    Matrix temp(rows_, m.cols_);
    for (int i = 0; i < temp.rows_; ++i) {
        for (int j = 0; j < temp.cols_; ++j) {
            for (int k = 0; k < cols_; ++k) {
                temp.p[i][j] += (p[i][k] * m.p[k][j]);
            }
        }
    }
    return (*this = temp);
}


Matrix& Matrix::operator/=(double num)
{
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            p[i][j] /= num;
        }
    }
    return *this;
}

Matrix Matrix::createIdentity(int size)
{
    Matrix temp(size, size);
    for (int i = 0; i < temp.rows_; ++i) {
        for (int j = 0; j < temp.cols_; ++j) {
            if (i == j) {
                temp.p[i][j] = 1;
            } else {
                temp.p[i][j] = 0;
            }
        }
    }
    return temp;
}

Matrix Matrix::createZero(int size)
{
    Matrix temp(size, size);
    for (int i = 0; i < temp.rows_; ++i) {
        for (int j = 0; j < temp.cols_; ++j) {
            temp.p[i][j] = 0;
        }
    }
    return temp;
}

void Matrix::allocSpace()
{
    p = new double*[rows_];
    for (int i = 0; i < rows_; ++i) {
        p[i] = new double[cols_];
    }
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
    Matrix temp(m1);
    return (temp *= m2);
}
//------matrix.cpp END------

//------point.cpp------
point::point(){
    double arr[] = { 0.0,
                     0.0,
                     0.0,
                     1.0 };
    x = Matrix(4,1,arr);
}
point::point(double _x,double _y,double _z){
    double arr[] = { _x,
                     _y,
                     _z,
                    1.0 };
    x = Matrix(4,1,arr);
}
point::point(const Matrix& mat){
    x = mat;
}
point::point(const point& other){
    this->x = other.x;
}
point& point::operator=(const point& other){
    this->x = other.x;
    return *this;
}

void point::makeHomogeneous(){
    if(this->x(3,0) != 0)
        this->x /= this->x(3,0);
}

std::ostream& operator<<(std::ostream& sout, const point& p){
    Matrix mat = p.x;
    sout << mat(0,0) << " " << mat(1,0) << " " << mat(2,0);
    return sout;
}

point operator*(const Matrix& m, const point& p){
    point out = point(m*p.x);
    out.makeHomogeneous();
    return out;
}
//------point.cpp END------

//------vector.cpp------
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
//------vector.cpp END------

//------matrix generating functions--------
Matrix getTranslateMatrix(double tx,double ty,double tz){
    double arr[] = { 1.0, 0.0, 0.0, tx ,
                     0.0, 1.0, 0.0, ty ,
                     0.0, 0.0, 1.0, tz ,
                     0.0, 0.0, 0.0, 1.0 };
    return Matrix(4,4,arr);
}

Vector Rotate(Vector x,Vector a,double angleDeg){
    double theta = DEGtoRED(angleDeg);
    //Rodrigues' rotation formula
    return x*cos(theta)
         + a*(1.0 - cos(theta) )*a.dot(x)
         + a.cross(x)*sin(theta);
}

Matrix getRotationMatrix(double angleDeg,double ax,double ay,double az){
    Vector a(ax,ay,az);
    a = a.normalize();
    Vector c1 = Rotate(Vector(1,0,0),a,angleDeg);
    Vector c2 = Rotate(Vector(0,1,0),a,angleDeg);
    Vector c3 = Rotate(Vector(0,0,1),a,angleDeg);

    double arr[] = { c1.x, c2.x, c3.x, 0.0 ,
                     c1.y, c2.y, c3.y, 0.0 ,
                     c1.z, c2.z, c3.z, 0.0 ,
                      0.0,  0.0,  0.0, 1.0 };
    return Matrix(4,4,arr);
}

Matrix getViewMatrix(double  eyeX,double  eyeY,double  eyeZ,
                     double lookX,double lookY,double lookZ,
                     double   upX,double   upY,double   upZ )
{

    Vector look(lookX,lookY,lookZ);
    Vector  eye( eyeX, eyeY, eyeZ);
    Vector   up(  upX,  upY,  upZ);

    Vector l = look - eye;
    l = l.normalize();

    Vector r = l.cross(up);
    r = r.normalize();

    Vector u = r.cross(l);


    double arr[] = { 1.0, 0.0, 0.0,-eyeX ,
                     0.0, 1.0, 0.0,-eyeY ,
                     0.0, 0.0, 1.0,-eyeZ,
                     0.0, 0.0, 0.0, 1.0 };
    Matrix T(4,4,arr);

    double brr[] = { r.x, r.y, r.z, 0.0 ,
                     u.x, u.y, u.z, 0.0 ,
                    -l.x,-l.y,-l.z, 0.0,
                     0.0, 0.0, 0.0, 1.0 };
    Matrix R(4,4,brr);

    return R*T;
}

Matrix getProjMatrix(double fovY,double aspectRatio,
                     double near,double far         )
{
    double fovX = fovY * aspectRatio;

    fovX = DEGtoRED(fovX);
    fovY = DEGtoRED(fovY);

    double    t = near * tan(fovY/2);
    double    r = near * tan(fovX/2);

    double arr[] =
    { near/r,    0.0,          0.0,                 0.0 ,
         0.0, near/t,          0.0,                 0.0 ,
         0.0,    0.0,-(far+near)/(far-near),-(2*far*near)/(far-near),
         0.0,    0.0,         -1.0,                 0.0              };

    return Matrix(4,4,arr);
}

Matrix getScaleMatrix(double sx,double sy,double sz){

    double arr[] = {  sx, 0.0, 0.0, 0.0 ,
                     0.0,  sy, 0.0, 0.0 ,
                     0.0, 0.0,  sz, 0.0 ,
                     0.0, 0.0, 0.0, 1.0 };
    return Matrix(4,4,arr);
}
//------matrix generating functions END--------

//------main function------
int main(int argc, char const *argv[]) {

    ifstream scene("scene.txt");

    ofstream stage1("stage1.txt");
    ofstream stage2("stage2.txt");
    ofstream stage3("stage3.txt");

    string line;

    stack<Matrix> S;
    S.push(Matrix::createIdentity(4));


    Matrix ViewMatrix;
    double lookX,lookY,lookZ;
    double  eyeX, eyeY, eyeZ;
    double   upX,  upY,  upZ;

    Matrix ProjMatrix;
    double fovY,aspectRatio,near,far;

    for(int i = 1;getline(scene,line);i++){

        while ( line[(int)line.length()-1] == '\r' ||
                line[(int)line.length()-1] == '\n'    )
            line.pop_back();

        if(i == 1){
            sscanf(line.c_str(),"%lf%lf%lf", &eyeX,&eyeY,&eyeZ);
        }else if(i == 2){
            sscanf(line.c_str(),"%lf%lf%lf", &lookX,&lookY,&lookZ);
        }else if(i == 3){

            sscanf(line.c_str(),"%lf%lf%lf", &upX,&upY,&upZ);

            ViewMatrix = getViewMatrix( eyeX, eyeY, eyeZ,
                                       lookX,lookY,lookZ,
                                         upX,  upY,  upZ );

        }else if(i == 4){

            sscanf(line.c_str(),"%lf%lf%lf%lf", &fovY,&aspectRatio,
                                                &near,&far         );

            ProjMatrix =  getProjMatrix(fovY,aspectRatio,
                                        near,far         );

        }else{

            if(line == string("triangle")){

                double Vx,Vy,Vz;
                point p,_p;

                for (int n = 0; n < 3; n++) {
                    getline(scene,line);i++;
                    sscanf(line.c_str(),"%lf%lf%lf", &Vx,&Vy,&Vz);
                    p  = point(Vx,Vy,Vz);
                    _p = S.top()*p;
                    stage1 << setprecision(7) << fixed << _p << endl;

                    _p = ViewMatrix*_p;
                    stage2 << setprecision(7) << fixed << _p << endl;

                    _p = ProjMatrix*_p;
                    stage3 << setprecision(7) << fixed << _p << endl;
                }

                stage1 << endl;
                stage2 << endl;
                stage3 << endl;

            }else if(line == string("translate")){

                double tx,ty,tz;

                getline(scene,line);i++;

                sscanf(line.c_str(),"%lf%lf%lf", &tx,&ty,&tz);
                Matrix S_top = S.top();S.pop();
                Matrix TM    = getTranslateMatrix(tx,ty,tz);
                S.push(S_top*TM);

            }else if(line == string("scale")){

                double sx,sy,sz;

                getline(scene,line);i++;

                sscanf(line.c_str(),"%lf%lf%lf", &sx,&sy,&sz);
                Matrix S_top = S.top();S.pop();
                Matrix SM    = getScaleMatrix(sx,sy,sz);
                S.push(S_top*SM);

            }else if(line == string("rotate")){

                double angleDeg,ax,ay,az;

                getline(scene,line);i++;

                sscanf(line.c_str(),"%lf%lf%lf%lf", &angleDeg, &ax,&ay,&az);
                Matrix S_top = S.top();S.pop();
                Matrix RM    = getRotationMatrix(angleDeg, ax,ay,az);
                S.push(S_top*RM);

            }else if(line == string("push")){
                S.push(S.top());
            }else if(line == string("pop")){
                S.pop();
            }else if(line == string("end")){
                break;
            }

        }
    }

     scene.close();
    stage1.close();
    stage2.close();
    stage3.close();

    return 0;
}
