#include "matrix.h"
#include "point.h"
#include <bits/stdc++.h>

using namespace std;

Matrix getTranslateMatrix(double tx,double ty,double tz){
    double arr[] = { 1.0, 0.0, 0.0, tx ,
                     0.0, 1.0, 0.0, ty ,
                     0.0, 0.0, 1.0, tz ,
                     0.0, 0.0, 0.0, 1.0 };

    return Matrix(4,4,arr);
}

Vector Rotate(Vector x,Vector a,double angleDeg){
    double theta = DEGtoRED(angleDeg);
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

int main(int argc, char const *argv[]) {

    ifstream scene("scene.txt");

    ofstream stage1("stage1.txt");
    ofstream stage2("stage2.txt");
    ofstream stage3("stage3.txt");

    string line;

    stack<Matrix> S;
    S.push(Matrix::createIdentity(4));


    double lookX,lookY,lookZ;
    double  eyeX, eyeY, eyeZ;
    double   upX,  upY,  upZ;
    Matrix ViewMatrix;

    double fovY,aspectRatio,near,far;
    Matrix ProjMatrix;

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
