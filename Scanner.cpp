#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Scanner.h"
using namespace std;

vector<double> TCD; // Table const (double)
vector<string> TCS;
vector<IdenT> TID;
vector<Lex> sequence_lex;

Lex::Lex(): t_lex(LEX_NULL), v_lex(0){}
Lex::Lex(type_of_lex t, int v=0): t_lex(t), v_lex(v){}
type_of_lex Lex::get_type() const {return t_lex;}
int Lex::get_value()const {return v_lex;}
bool Lex::operator==(const Lex& a) const {
    return (t_lex == a.t_lex) && (v_lex == a.v_lex);
}
bool Lex::operator!=(const Lex& a) const {
    return !(*this == a);
}
void Lex::swap(Lex& a, Lex& b){
    std::swap(a.t_lex, b.t_lex);
    std::swap(a.v_lex, b.v_lex);
}
Lex& Lex::operator=(Lex a){
    swap(*this, a);
    return *this;
}


IdenT::IdenT(){}
IdenT::IdenT(const string c): name(c){}
bool IdenT::operator==(const string& s){
    return name == s;
}

void Scanner::gc(){
    c = fgetc(file);
    //cout << c;
}
int Scanner::look(string& buf, char** tabel){
    int i = 1;
    while(tabel[i]){
        if (buf == tabel[i]) return i;
        i++;
    }
    return 0;
}
template<typename TYPE>
int Scanner::put(TYPE buf, vector<TYPE> T){
    typename vector<TYPE>::iterator i;
    if ((i = find(T.begin(), T.end(), buf)) != T.end()) return i - T.begin();
    T.push_back(buf);
    return T.size() - 1;        
}
int Scanner::put(string buf){
    vector<IdenT>::iterator i;
    if ((i = find(TID.begin(), TID.end(), buf)) != TID.end()) return i - TID.begin();
    TID.push_back(IdenT(buf));
    return TID.size() - 1;
}

Scanner::Scanner(const char* s=nullptr){
    file = fopen(s, "r");
    state = H;
    if (file == nullptr) throw "can't open file\n";
    gc();
}
Scanner::~Scanner(){}
// Таблица служебных слов
char* Scanner::TW[] = {
    //0        1        2       3         4         5       6        7      8       9    10     11      12     13     14
    nullptr, "int", "string", "real", "program", "while", "write", "read", "and", "or", "not", "do", "break", "if", "else"
};
// Таблица служебных разделителей
char* Scanner::TD[] = {
    //0       1    2    3    4    5   6    7    8   9    10   11   12  13   14   15     16    17    18
    nullptr, ";", "+", "-", "/", "*","%", ",", "(", ")", "{", "}","=", ">", "<", "==", "!=", ">=", "<="
};



Lex Scanner::get_lex(){
    string buf = "";
    int i = 0;
    double d = 0, count_D = .1;
    while(state != ERR){
        switch (state)
        {
        case H:
            if (c == ' ' || c == '\n' || c == '\t' || c == '\r'){
                gc();
            } else if (isalpha(c)){
                buf += c;
                state = IDENT;
                gc();
            } else if (isdigit(c)){
                i = c - '0';
                state = NUMB;
                gc();
            } else if (c == '>' || c == '<' || c == '='){
                buf += c;
                state = ALE;
                gc();
            } else if (c == '/'){ 
                state = PREDCOM;
                buf += c;
                gc();
            }else if (c == -1) {
                return Lex(LEX_FIN); //EOF_LEX
            } else if (c == '!'){
                state = NEQ;
                gc();
            } else {
                state = DELIM;
            }
            break;
        case IDENT:
            if (isalpha(c) || isdigit(c)){
                buf += c;
                gc();
            } else {
                state = H;
                int j = look(buf, TW);
                if (j == 0) {
                    return Lex(LEX_ID, put(buf));
                } else return Lex(LEX_WORD, j);
            }
            break;
        case NUMB:
            if (isdigit(c)){
                i = i *10 + c - '0';
                gc();
            } else if (c == '.'){
                d = i;
                state = FLOAT;
                gc();
            } else {
                state = H;
                return Lex(LEX_NUM, i);
            }
            break;
        case FLOAT:
            if (isdigit(c)){
                d += (c - '0') * count_D;
                count_D /= 10;
                gc();
            } else {
                state = H;
                return Lex(LEX_FLOAT, put(d, TCD));
            }
            break;
        case PREDCOM:
            if (c == '*') {state = COM; gc();}
            else { state = H; buf += c; return Lex(LEX_DELIM, look(buf, TD));}
            break; 
        case COM:
            if (c == '*') state = AFTCOM;
            else if (c == -1) throw c;
            gc();
            break;
        case AFTCOM:
            if (c == '/') state = H;
            else if (c == -1) throw c;
            else state = COM;
            gc();
            break;
        case ALE:
            state = H;
            if (c == '='){
                buf += c;
                gc();
                return Lex(LEX_DELIM, look(buf, TD));
            }
            return Lex(LEX_DELIM, look(buf, TD));
        case NEQ:
            if (c == '=') {
                state = H;
            } else throw c;
            break;
        case DELIM:
            switch (c)
            {
            case '+':
            case '-':
            case '*':
            case '%':
            case '(':
            case ')':
            case '{':
            case '}':
            case ';':
            case ',':
                state = H;
                buf += c;
                gc();
                return Lex(LEX_DELIM, look(buf, TD));
            case '"':
                state = STR;
                gc();
                break;
            default:
                throw c;
            }
            break;
        case STR:
            if (c == '\\') state = STRSLASH;
            else if (c == '"') {
                state = H; gc(); 
                return Lex(LEX_STR, put(buf, TCS));
            } else if (c == -1) throw c;
            else buf += c;
            gc();
            break;
        case STRSLASH:
            if (c == '\\' || c == '"') buf += c;
            else if (c == -1) throw c;
            gc();
            state = STR;
            break;
        }
    }
    return Lex(LEX_NULL);
}

std::ostream& operator<<(std::ostream& out, Lex& l){
    out << '(' << l.t_lex << ',' << l.v_lex << ',';
    return  cout << ')';
}

void first_pass(Scanner s){
    try{
        Lex tmp(LEX_NULL);
        while ((tmp = s.get_lex()) != Lex(LEX_FIN)){
            sequence_lex.push_back(tmp);
        }
        sequence_lex.push_back(tmp);
    } catch(char c){
        for(Lex l: sequence_lex) cout << l.get_type();
        cout << "Error " << c << endl;
        exit(0);
    }
}