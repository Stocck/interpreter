#pragma once

#include <vector>
#include <string>
enum type_of_lex { LEX_NULL,
    LEX_WORD, LEX_DELIM, LEX_EXP,
    //EX_INT, LEX_STRING, LEX_REAL, LEX_PRORAM, LEX_WHILE, LEX_WRITE, LEX_READ, LEX_AND, LEX_OR, LEX_NOT, LEX_DO, LEX_BREAK, LEX_IF, LEX_ELSE,
    //TW = "int", "string", "real", "program", "while", "write", "read", "and", "or", "not", "do", "break", "if", "else"
    LEX_ID, LEX_NUM, LEX_FLOAT, LEX_STR, LEX_FIN,
};

class Lex{
    type_of_lex t_lex;
    int v_lex;
public:
    Lex();
    Lex(type_of_lex, int);
    type_of_lex get_type() const;
    int get_value() const;
    static void swap(Lex&, Lex&);
    Lex& operator=(Lex);
    bool operator==(const Lex& a) const;
    bool operator!=(const Lex& a) const;
    friend std::ostream& operator<<(std::ostream&, Lex&);
};

class IdenT{
    std::string name;
    bool declare = false;
    type_of_lex t;
    bool assign = false;
    int value;
public:
    IdenT();
    IdenT(const std::string);
    bool operator==(const std::string&);
};

class Scanner{
    FILE* file;
    char c;
    int state;
    enum {H, IDENT, NUMB, FLOAT, ALE, NEQ, DELIM, STR, STRSLASH,PREDCOM, COM, AFTCOM, ERR};
    void gc();
    int look(std::string& buf, char** tabel);
    template<typename TYPE>
    int put(TYPE buf, std::vector<TYPE>);
    int put(std::string buf);
public:
    static char *TW[], *TD[];
    
    Scanner(const char* s);
    ~Scanner();
    Lex get_lex();
};
void first_pass(Scanner);

extern std::vector<double> TCD; // Table const (double)
extern std::vector<std::string> TCS;
extern std::vector<IdenT> TID;
extern std::vector<Lex> sequence_lex;
