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
        void push_back(Token);
        int size();
        Token& at(int loc);
        std::string getStringValue();
        void append(Token t);
        bool end() { return currentInc > size()-1; }
        int scope(){return tokens[currentInc].scopenumber;}
        void reset();
        void erase(std::vector<Token>::iterator);
        int current(){ return currentInc; }
        std::vector<Token>::iterator front(){ return tokens.begin(); }
        std::vector<Token>::iterator back(){ return tokens.end(); }
        std::string value();
        std::vector<Token> tokens;
        TokenSegment eatLine(TokenSegment&);
        TokenSegment eatIndented(TokenSegment&);
    protected:
    private:

    int currentInc = 0;
    std::string cached;
};

#endif // TOKENSEGMENT_H
