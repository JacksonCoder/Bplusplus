#include "Libraries/shared.h"
#include "Structures/tokenType.h"
#include "Structures/File.h"
#include "Structures/ASTNode.h"
#include "Structures/ASTTree.h"
#include "Structures/Compiler.h"
#include "Structures/TokenSegment.h"
std::string startupMessage = "Welcome to B++ v0.2.\nPlease enter the name of a file to open:\n";
/*
Work that still needs to be done:
*Constructing ASTNodes
*ASTNode type determination
File opening chaos...
And debugging...
SO DON'T GET DISTRACTED! GET TO WORK!

void startProcess(File *readfile,File *writefile)
{
preParseContents(readfile);
ASTTree bt = assembleTree(readfile);
/*
bt->validateTree();
std::cout << bt->dumpLog() << std::endl;

bt.getRoot()->assemble();
std::cout << bt.getRoot()->getResult() << std::endl;
}
*/
int main(int argc,char** argv)
{
/*
    runcommand : cd src; clang++ -o ../bin/Debug/Compiler Structures/ASTNode.cpp Structures/ASTTree.cpp Structures/Compiler.cpp Structures/Error.cpp Structures/File.cpp Structures/Token.cpp Structures/TokenLexer.cpp Structures/TokenSegment.cpp Structures/TVar.cpp Structures/Module.cpp Compiler/fail.cpp main.cpp -std=c++11 -Wall -stdlib=libc++;cd ..
    tokensegmentTest : cd src; clang++ -o ../bin/Debug/Compiler Structures/TokenSegment.cpp main.cpp -std=c++1y stdlib=libc++
*/  
    //Current stage: testing
    #ifndef TESTING
    Compiler c(argc,argv);
    c.run();
    
    //std::cout << "Compilation sucessful!\n";
    #else
    //put tests
    #endif
    return 0;
}
