#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>

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

#endif
