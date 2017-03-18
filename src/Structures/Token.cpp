#include "Token.h"

Token::Token()
{
    //ctor
}

Token::~Token()
{
    //dtor
}

Token::Token(TokenType t,std::string s) : value(s), tokentype(t)
{}