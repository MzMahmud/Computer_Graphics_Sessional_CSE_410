#include "matrix.h"
#include "point.h"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char const *argv[]) {
    double arr[] = { 1,2,3,4,
                     1,0,3,4,
                     1,2,3,0,
                     0,2,3,4 };

    Matrix m(4,4,arr);
    point p(7,4,8);

    cout << m*p << endl;
    return 0;
}
