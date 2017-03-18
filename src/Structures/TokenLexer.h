#ifndef TOKENLEXER_H
#define TOKENLEXER_H
#include "File.h"
#include "tokenType.h"
#include "TokenSegment.h"
class TokenLexer
{
    public:
        TokenLexer();
        ~TokenLexer();
        void construct(std::string);
        TokenSegment getOutput() { return out; }
    protected:
    private:
        std::string source;
        TokenSegment out;
};

#endif // TOKENLEXER_H
