#ifndef MODULE_H
#define MODULE_H
#include "../src/Libraries/shared.h"
#include "TokenLexer.h"
#include "ASTTree.h"
class ASTTree;
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
