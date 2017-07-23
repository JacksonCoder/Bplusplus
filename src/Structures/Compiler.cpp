#include "../../Include/Compiler.h"

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
        << "Created by Jackson Lewis. Last updated July 22nd 2017.\n(c) Jackson Lewis 2017. All Rights Reserved.";
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
    if(outCount<1) fail("No output file specified");
    if(outCount>1) fail("You can only declare one output file");
    if(std::find(arguments.begin(),arguments.end(),"-debug")!=arguments.end())
    {
        std::cout<<"Debug mode enabled\n";
        isdebug = true;
    }
    sourceModule = new Module(in,out);
    sourceModule->build();
    sourceModule->output();
}

void Compiler::run()
{
        if(arguments.size() < 1)
        {
          help();
          return;
        }
        std::string firstarg = arguments[0];
        if(firstarg=="version")
        {
            displayVersion();
            return;
        }
        if(firstarg=="compile")
        {
            standardCompileSetup();
            return;
        }
        if(firstarg=="help")
        {
          help();
          return;
        }
        fail("Invalid first argument!");
}

void Compiler::help()
{
  std::cout<<
    "This is the B++ Compiler command line tool. A command for this compiler looks like this:" <<
    "<path to compiler executable> <command> <options/arguments>\n\n" <<
    "The current commands for this compiler are:\n" <<
    "help -> displays help for the compiler\n" <<
    "version -> displays the version number\n" <<
    "compile -> runs the compiler\n\n" <<
    "The current options/arguments are:\n" <<
    "-source <filename> -> specifies the source file to read from when compiling (it reads the file <filename>)\n" <<
    "-out <filename> -> specifies the output file to dump the results into after compiling (it reads the file <filename>)\n" <<
    "-debug -> Enables the debug mode and prints out cryptic logging details (for testing purposes)\n"
    << std::endl;
    displayVersion();
}

Compiler::~Compiler()
{
    //clean up
    delete sourceModule;
}
