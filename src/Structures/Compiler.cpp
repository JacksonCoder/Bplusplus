#include "Compiler.h"

Compiler::Compiler(int argc,char** argv)
{
    for(int i = 1; i < argc;i++)
        {
        arguments.push_back(argv[i]);
        }
}

void Compiler::displayVersion()
{
        std::cout<<"=========================\nThe current version for this compiler is "<<VERSION_NUMBER<<"\n";
}

void Compiler::standardCompileSetup()
{

}

void Compiler::prepare()
{
        std::string firstarg = arguments[0];
        if(firstarg=="version")
        {
            displayVersion();
            exit(0);
        }
        if(firstarg=="compile")
        {
            standardCompileSetup();
        }
        else fail("Invalid first argument!");
}

Compiler::~Compiler()
{
    //clean up
}
