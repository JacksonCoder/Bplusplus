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
    unsigned int sourceCount = 0;
    std::cout<<"SourceCount:"<<sourceCount<<std::endl;
    while(std::find(arguments.begin(),arguments.end(),"-source")!=arguments.end())
    {
        sourceCount++;
        std::vector<std::string>::iterator iter = std::find(arguments.begin(),arguments.end(),"-source") + 1;
        std::cout<<"Check this out:"<<*iter<<std::endl;
        //File* sourceFile = new File;
        //TokenLexer tl(*sourceFile);
        //tl.construct(*iter);
        arguments.erase(iter-1,iter);
    }
    std::cout<<"SourceCount:"<<sourceCount<<std::endl;
    if(sourceCount<1) fail("No source file specified");
    if(sourceCount>1) fail("You can only declare one source file");
    while(std::find(arguments.begin(),arguments.end(),"-include")!=arguments.end())
    {
        std::vector<std::string>::iterator iter = std::find(arguments.begin(),arguments.end(),"-include") + 1;
        std::cout<<"Check this out:"<<*iter<<std::endl;
        arguments.erase(iter-1,iter);
    }
    //NEXT: add module class, and then use them as a wrapper for ASTTrees
}

void Compiler::run()
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
