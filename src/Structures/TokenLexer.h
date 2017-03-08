#ifndef TOKENLEXER_H
#define TOKENLEXER_H
#include "File.h"

enum Token {
    NAME,
    LTERM,
    OPAREN,
    CPAREN,
    COLON,
    COMMA,
};


class TokenLexer
{
    public:
        TokenLexer(File&);
        ~TokenLexer();
    protected:
    private:
        File& lexerFile;
};

#endif // TOKENLEXER_H
