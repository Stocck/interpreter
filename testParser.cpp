#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Scanner.h"
using namespace std;

class Parser{
    Lex curr_lex;
    Scanner scan;

    void gl(){curr_lex = scan.get_lex();cout << curr_lex;}
    void P();
    void DETs();void DET();
    void TYPE(); void VAR();
    void CONST();
    void INT(); void SIGN(); 
    void STR(); void REAL();
    void OPs(); void OP();
    void COMOP(); void EXPOP();
    //EXP
    void A();
    void B();
    void C();
    void D();
    void E();
    void F();
    void G();
    

public:
    Parser(Scanner& s): scan(s), curr_lex(LEX_NULL, 0){}
    void analyze();
};

void Parser::P(){
    if(curr_lex == Lex(LEX_WORD,4)) {// program
        gl();
        if (curr_lex != Lex(LEX_DELIM,10)) throw curr_lex; // {
        gl();
        DETs();
        OPs();
        if (curr_lex == Lex(LEX_DELIM,11)) // }
            gl();
    }
    else throw curr_lex;
}
void Parser::DETs(){
    if(curr_lex == Lex(LEX_WORD, 1) || curr_lex == Lex(LEX_WORD, 2) || curr_lex == Lex(LEX_WORD, 3)) {//int string real
        DET();
        while(curr_lex == Lex(LEX_DELIM,1)){ // ;
                gl();
                if(curr_lex == Lex(LEX_WORD, 1) || curr_lex == Lex(LEX_WORD, 2) || curr_lex == Lex(LEX_WORD, 3)) 
                    DET();
        }
    }
}
void Parser::DET(){
    TYPE();
    VAR();
    while(curr_lex == Lex(LEX_DELIM,7)){ // ,
        gl();
        VAR();
    }
}
void Parser::TYPE(){
    if (curr_lex == Lex(LEX_WORD, 1)) // int
        gl();
    else if (curr_lex == Lex(LEX_WORD, 2)) // string
        gl();
    else if (curr_lex == Lex(LEX_WORD, 3)) // real
        gl();
    else throw curr_lex;

} 
void Parser::VAR(){
    if (curr_lex.get_type() == LEX_ID){
        gl();
        if (curr_lex == Lex(LEX_DELIM, 12)){
            gl();
            CONST();
        }
    } else throw curr_lex;
}
void Parser::CONST(){
    if (curr_lex.get_type() == LEX_STR){
        gl();
    } else if (curr_lex == Lex(LEX_DELIM, 2) || curr_lex == Lex(LEX_DELIM, 3)){
        Lex save_sign = curr_lex;
        gl();
        if (curr_lex.get_type() == LEX_FLOAT)
            gl();
        else if (curr_lex.get_type() == LEX_NUM)
            gl();
        else throw curr_lex;
    } else if (curr_lex.get_type() == LEX_FLOAT)
        gl();
    else if (curr_lex.get_type() == LEX_NUM)
        gl();
    else throw curr_lex;
}
// Реализованно внутри CONST
void Parser::INT(){} 
void Parser::SIGN(){} 
void Parser::STR(){} 
void Parser::REAL(){}
//

void Parser::OPs(){
    bool f = curr_lex == Lex(LEX_WORD, 13) || curr_lex == Lex(LEX_WORD, 5) || curr_lex == Lex(LEX_WORD, 11)
        || curr_lex == Lex(LEX_WORD, 7) || curr_lex == Lex(LEX_WORD, 6) || curr_lex == Lex(LEX_DELIM, 10)
        || curr_lex == Lex(LEX_WORD, 11) || curr_lex == Lex(LEX_DELIM, 8) || curr_lex.get_type() == LEX_ID
        || curr_lex.get_type() == LEX_NUM ||curr_lex.get_type() == LEX_STR ||curr_lex.get_type() == LEX_FLOAT;
    while (f){
        OP();
        f = curr_lex == Lex(LEX_WORD, 13) || curr_lex == Lex(LEX_WORD, 5) || curr_lex == Lex(LEX_WORD, 11)
        || curr_lex == Lex(LEX_WORD, 7) || curr_lex == Lex(LEX_WORD, 6) || curr_lex == Lex(LEX_DELIM, 10)
        || curr_lex == Lex(LEX_WORD, 11) || curr_lex == Lex(LEX_DELIM, 8) || curr_lex.get_type() == LEX_ID
        || curr_lex.get_type() == LEX_NUM ||curr_lex.get_type() == LEX_STR ||curr_lex.get_type() == LEX_FLOAT;
    }
}
void Parser::OP(){
    if(curr_lex == Lex(LEX_WORD, 13)) {//if
        gl();
        if (curr_lex != Lex(LEX_DELIM, 8)) throw curr_lex;//(
        gl();
        A();
        if (curr_lex != Lex(LEX_DELIM, 9)) throw curr_lex;//)
        gl();
        OP();
        if(curr_lex != Lex(LEX_WORD, 14)) throw curr_lex;//else
        gl();
        OP();
    } else if(curr_lex == Lex(LEX_WORD, 5)){//while
        gl();
        if (curr_lex != Lex(LEX_DELIM, 8)) throw curr_lex;//(
        gl();
        A();
        if (curr_lex != Lex(LEX_DELIM, 9)) throw curr_lex;//)
        gl();
        OP();
    } else if (curr_lex == Lex(LEX_WORD, 11)){//do
        gl();
        OP();
        if(curr_lex != Lex(LEX_WORD, 5)) throw curr_lex;//while
        gl();
        if (curr_lex != Lex(LEX_DELIM, 8)) throw curr_lex;//(
        gl();
        A();
        if (curr_lex != Lex(LEX_DELIM, 9)) throw curr_lex;//)
        gl();
        if (curr_lex != Lex(LEX_DELIM, 1)) throw curr_lex;//;
        gl();
    } else if (curr_lex == Lex(LEX_WORD, 7)){//read
        gl();
        if (curr_lex != Lex(LEX_DELIM, 8)) throw curr_lex;//(
        gl();
        if (curr_lex.get_type() != LEX_ID) throw curr_lex;//IDENT
        gl();
        if (curr_lex != Lex(LEX_DELIM, 9)) throw curr_lex;//)
        gl();
        if (curr_lex != Lex(LEX_DELIM, 1)) throw curr_lex; //;
        gl();
    } else if (curr_lex == Lex(LEX_WORD, 6)){//write
        gl();
        if (curr_lex != Lex(LEX_DELIM, 8)) throw curr_lex;//(
        gl();
        if (curr_lex.get_type() != LEX_ID) throw curr_lex;//IDENT
        gl();
        while(curr_lex == Lex(LEX_DELIM, 7)){//,
            gl();
            if (curr_lex.get_type() != LEX_ID) throw curr_lex;//IDENT
            gl();
        }
        if (curr_lex != Lex(LEX_DELIM, 9)) throw curr_lex;//)
        gl();
        if (curr_lex != Lex(LEX_DELIM, 1)) throw curr_lex; //;
        gl();
    } else if (curr_lex == Lex(LEX_DELIM, 10)){ //COMOP
        gl();
        OPs();
        if (curr_lex != Lex(LEX_DELIM, 11)) throw curr_lex; 
        gl();
    } else if (curr_lex == Lex(LEX_WORD, 12)){//break
        gl();
        if (curr_lex != Lex(LEX_DELIM, 1)) throw curr_lex; //;
        gl();
    } 
    else {EXPOP();}
}
void Parser::COMOP(){} 
void Parser::EXPOP(){
    A();
    if (curr_lex != Lex(LEX_DELIM, 1)) throw curr_lex; // ;
    gl();
}
//()=
void Parser::A() {
    if (curr_lex.get_type()==LEX_ID) {
        gl(); 
        if (curr_lex==Lex(LEX_DELIM, 12)) { // =
            gl();
            A();
        } else {
            gl();
            B();
        }
    } else B();
}
//OR
void Parser::B(){
    C();
    while (curr_lex == Lex(LEX_WORD, 9)){
        gl();
        C();
    }
}
//AND
void Parser::C(){ 
    D();
    while (curr_lex == Lex(LEX_WORD, 8)){
        gl();
        D();
    }
}
//< > <= >= == !=
void Parser::D () { 
    E();
    while (curr_lex == Lex(LEX_DELIM, 14) || curr_lex == Lex(LEX_DELIM, 13) || curr_lex == Lex(LEX_DELIM, 18)
    || curr_lex == Lex(LEX_DELIM, 17) || curr_lex == Lex(LEX_DELIM, 15) || curr_lex == Lex(LEX_DELIM, 16)) {
        gl(); 
        E(); 
    }
}
// + -
void Parser::E() {
    F();
    while (curr_lex == Lex(LEX_DELIM, 2) || curr_lex == Lex(LEX_DELIM, 3)) {
        gl();
        F();
    }
}
// * /
void Parser::F() {
    G();
    while (curr_lex == Lex(LEX_DELIM, 5) || curr_lex == Lex(LEX_DELIM, 4)) {
        gl();
        G();
    }
}
// not const (empty)
void Parser::G() {
    if (curr_lex.get_type() == LEX_ID) {
        gl();
    }
    else if ( curr_lex.get_type() == LEX_NUM ) {
        gl();
    }
    else if (curr_lex.get_type()==LEX_STR) {
        gl();
    }
    else if ( curr_lex.get_type() == LEX_FLOAT) {
        gl();
    }
    else if (curr_lex == Lex(LEX_DELIM, 8)){ // (
        gl();
        A();
        if (curr_lex != Lex(LEX_DELIM, 9)) throw curr_lex; // )
        gl();
    } else throw curr_lex;
}



void Parser::analyze(){
    gl(); P();
}


void second_pass(Parser& p){
    try{
        p.analyze();
    } catch(Lex l){
        cout << l.get_type() << " " << l.get_value() << endl;
        exit(0);
    }
}

int main(int argc, char** argv){
    try{
        Scanner s(argv[1]);
        Parser p(s);
        second_pass(p);
        cout << "\n ACSESS\n";
    } catch(char const* s){
        cout << s;
    }
    return 0;
}