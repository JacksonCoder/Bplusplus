#include "../Libraries/shared.h"
#include "../../Include/File.h"
#include "../../Include/ASTNode.h"
#include "../../Include/ASTTree.h"
#include "../../Include/Compiler.h"
#include "../../Include/TokenSegment.h"


int main(int argc,char** argv)
{
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
