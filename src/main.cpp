#include "Libraries/shared.h"
#include "Compiler/parsing.h"
#include "Structures/tokenType.h"
#include "Structures/BPPError.h"
#include "Structures/BPPFile.h"
#include "Structures/BPPTNode.h"
#include "Structures/BPPTokenTree.h"

std::string startupMessage = "Welcome to B++ v0.2.\nPlease enter the name of a file to open:\n";

void startProcess(BPPFile *readfile,BPPFile *writefile)
{
preParseContents(readfile);
BPPTokenTree bt = assembleTree(readfile);
/*
bt->validateTree();
std::cout << bt->dumpLog() << std::endl;
*/
bt.getRoot()->assemble();
std::cout << bt.getRoot()->getResult() << std::endl;
}

int main()
{

    std::cout << startupMessage;
    std::string fileToOpen;
    std::cin >> fileToOpen;
    BPPFile sourceFile;
    sourceFile.read(fileToOpen);
    std::string fileToWrite;
    std::cout << "Enter the output filename:\n";
    std::cin >> fileToWrite;
    BPPFile compileFile;
    //compileFile.read(fileToWrite);
    startProcess(&sourceFile,&compileFile);
    /*
    if(errorOnCompiling.errorExists())
    {
        std::cout << "==========Error occured while compiling:=============\n" << errorOnCompiling.getMessage() <<'\n';
        return 0;
    }
    */
    std::cout << "Compilation sucessful!" + fileToWrite + '\n';
    return 0;
}
