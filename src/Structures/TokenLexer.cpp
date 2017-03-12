#include "TokenLexer.h"

TokenLexer::TokenLexer(File& f) : lexerFile(f)
{

}

TokenLexer::~TokenLexer()
{
    //dtor
}

void TokenLexer::construct(std::string filename)
{
    lexerFile.read(filename);
}
