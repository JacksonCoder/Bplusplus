#ifndef COMPILER_H
#define COMPILER_H
#include "../Libraries/shared.h"
#include "ASTTree.h"
#include "TokenLexer.h"
#define VERSION_NUMBER 0.3
class Compiler
{
    public:
        Compiler(int,char**);
        ~Compiler();
        void run();
        void lex();
        void finish();
    protected:
    private:
        std::vector<ASTTree*> modules;
        std::vector<std::string> arguments;
        void displayVersion();
        void standardCompileSetup();
};

#endif // COMPILER_H
