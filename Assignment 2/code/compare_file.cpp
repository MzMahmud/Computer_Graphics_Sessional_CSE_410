#include <bits/stdc++.h>
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"

using namespace std;

void trim(string &s){
    while(s[(int)s.length()-1] == '\r'
       || s[(int)s.length()-1] == '\n'
       || s[(int)s.length()-1] == ' '
       || s[(int)s.length()-1] == '\t' )

       s.pop_back();
}

int main(int argc, char const *argv[]) {
    if(argc != 3){
        cout << "Usage\n: ./compare.out file1 file2";
        return -1;
    }

    ifstream file1(argv[1]);
    ifstream file2(argv[2]);

    string line1,line2;
    bool allMatched = true;
    while( getline(file1,line1) &&
           getline(file2,line2)    ) {

        trim(line1);
        trim(line2);

        if(line1 != line2){
            printf(RED "CONFLICT: \n\t\t%s\n\t\t%s\n" RESET,line1.c_str(),line2.c_str());
            allMatched = false;
        }

    }
    if(allMatched){
        printf(GRN "%s == %s\n" RESET ,argv[1],argv[2]);
    }

    file1.close();
    file2.close();
    return 0;
}
