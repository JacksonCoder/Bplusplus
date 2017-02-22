#include "Libraries/shared.h"
#include "Structures/tokenType.h"
#include "Structures/BPPError.h"
#include "Structures/BPPFile.h"
#include "Structures/BPPTNode.h"
#include "Structures/BPPTokenTree.h"
#include "Compiler/parsing.h"
std::string startupMessage = "Welcome to B++ v0.2.\nPlease enter the name of a file to open:\n";

BPPError startProcess(BPPFile *readfile,BPPFile *writefile)
{
preParseContents(readfile);
BPPTokenTree bt = assembleTree(readfile);
if(bt.isError()) return bt.getError();

bt.setup();
bt.metaSetup();
BPPError someError = bt.getRoot()->assemble();
if(someError.errorExists()) return bt.getError();
std::cout << bt.getRoot()->getResult() << std::endl;
BPPError noError;
return noError;
}

int main()
{
    std::cout << startupMessage;
    std::string fileToOpen;
    std::cin >> fileToOpen;
    BPPFile sourceFile;
    sourceFile.read(fileToOpen);
    if(sourceFile.errorOccured())
    {
        std::cout << "The file you selected could not be open.\nExiting...";
        return 0;
    }
    std::string fileToWrite;
    std::cout << "Enter the output filename:\n";
    std::cin >> fileToWrite;
    BPPFile compileFile;
    //compileFile.read(fileToWrite);
    if(compileFile.errorOccured())
    {
        std::cout << "The file you selected could not be written to.\nExiting...";
        return 0;
    }

    BPPError errorOnCompiling = startProcess(&sourceFile,&compileFile);
    if(errorOnCompiling.errorExists())
    {
        std::cout << "==========Error occured while compiling:=============\n" << errorOnCompiling.getMessage() <<'\n';
        return 0;
    }
    std::cout << "Compilation sucessful!" + fileToWrite + '\n';
    return 0;
}
