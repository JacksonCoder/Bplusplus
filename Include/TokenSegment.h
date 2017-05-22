#ifndef TOKENSEGMENT_H
#define TOKENSEGMENT_H
#include "Token.h"
#include "../src/Libraries/shared.h"
#include "fail.h"
class TokenSegment
{
    public:
        TokenSegment();
        ~TokenSegment();
        TokenSegment(std::vector<Token> tokens): tokens(tokens) {}
        void next();
        Token get();
        TokenType type();
        void push_back(TokenType,std::string);
        int size();
        Token at(int loc);
        std::string getStringValue();
        void append(Token t);
        std::vector<Token> createUntil(std::initializer_list<TokenType> tokentypeil,unsigned int& i,TokenSegment& ts,bool processing_loop);
    protected:
    private:
    std::vector<Token> tokens;
    int currentInc = 0;
    std::string cached;
};

#endif // TOKENSEGMENT_H
