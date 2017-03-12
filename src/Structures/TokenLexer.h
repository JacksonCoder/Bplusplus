#ifndef TOKENLEXER_H
#define TOKENLEXER_H
#include "File.h"

class TokenLexer
{
    public:
        TokenLexer(File&);
        ~TokenLexer();
        void construct(std::string);
    protected:
    private:
        File& lexerFile;
};

#endif // TOKENLEXER_H
