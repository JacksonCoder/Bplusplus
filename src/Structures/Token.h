#ifndef TOKEN_H
#define TOKEN_H
#include "tokenType.h"
#include "../Libraries/shared.h"
class Token
{
    public:
        Token(TokenType,std::string,int);
        TokenType getType(){ return tokentype; }
        std::string getValue(){ return value; }
        Token();
        ~Token();
        unsigned int scopenumber;
    protected:
    private:
    std::string value;
    TokenType tokentype;
};
#endif // TOKEN_H
