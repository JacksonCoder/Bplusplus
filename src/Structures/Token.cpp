#include "Token.h"

Token::Token()
{
    //ctor
}

Token::~Token()
{
    //dtor
}

Token::Token(TokenType t,std::string s,int i) : value(s), tokentype(t), scopenumber(i)
{}