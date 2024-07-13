#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Scanner.h"
using namespace std;


int main(int argc, char** argv){
    try{
        Scanner s(argv[1]);
        first_pass(s);
        for (int i = 0; i < int(sequence_lex.size()); i++)
            cout << sequence_lex[i].get_type();
        cout << endl;
    } catch(char const* s){
        cout << s;
    }
    return 0;
}