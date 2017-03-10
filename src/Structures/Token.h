#ifndef TOKEN_H
#define TOKEN_H
#include "tokenType.h"
#include "../Libraries/shared.h"
class Token
{
    public:
        Token();
        ~Token();
    protected:
    private:
    std::string value;
    TokenType tokentype;
};

#endif // TOKEN_H
