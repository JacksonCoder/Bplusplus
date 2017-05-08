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
        std::cout<<"=========================\nThe current version for this compiler is "<<VERSION_NUMBER<<"\n"
        << "Created by Jackson Lewis. Last updated March 28th 2017.\n(c) Jackson Lewis 2017. All Rights Reserved.";
}

void Compiler::standardCompileSetup()
{
    unsigned int sourceCount = 0;
    std::string in,out;
    std::cout<<"SourceCount:"<<sourceCount<<std::endl;
    while(std::find(arguments.begin(),arguments.end(),"-source")!=arguments.end())
    {
        sourceCount++;
        std::vector<std::string>::iterator iter = std::find(arguments.begin(),arguments.end(),"-source") + 1;
        std::cout<<"Check this out:"<<*iter<<std::endl;
        in = *iter;
        arguments.erase(iter-2,iter);
    }
    if(sourceCount<1) fail("No source file specified");
    if(sourceCount>1) fail("You can only declare one source file");
    unsigned int outCount = 0;
    while(std::find(arguments.begin(),arguments.end(),"-out")!=arguments.end())
    {
        outCount++;
        std::vector<std::string>::iterator iter = std::find(arguments.begin(),arguments.end(),"-out") + 1;
        std::cout<<"Check this out:"<<*iter<<std::endl;
        out = *iter;
        arguments.erase(iter-2,iter);
    }
     if(sourceCount<1) fail("No output file specified");
    if(sourceCount>1) fail("You can only declare one output file");
    sourceModule = new Module(in,out);
    sourceModule->build();
    //sourceModule->output();
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
