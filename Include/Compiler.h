#ifndef COMPILER_H
#define COMPILER_H
#include "../src/Libraries/shared.h"
#include "TokenLexer.h"
#include "Module.h"
#define VERSION_NUMBER "0.3.12"
class Compiler
{
    public:
        Compiler(int,char**);
        ~Compiler();
        void run();
        void lex();
        void finish();
        void help();
    protected:
    private:
        Module* sourceModule;
        std::vector<std::string> arguments;
        void displayVersion();
        void standardCompileSetup();
};

#endif // COMPILER_H
