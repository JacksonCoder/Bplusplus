#ifndef MODULE_H
#define MODULE_H
#include "../Libraries/shared.h"
#include "TokenLexer.h"
#include "ASTTree.h"
//#include "ASTTree.h"
class Module 
{
    public:
    Module(std::string,std::string);
    ~Module(){}
    void build();
    void output();
    private:
    std::string infile;
    std::string outfile;
    ASTTree tree;
    TokenLexer lex;
};
#endif