#ifndef TOKENSEGMENT_H
#define TOKENSEGMENT_H
#include "Token.h"
#include "../Libraries/shared.h"
#include "../Compiler/fail.h"
class TokenSegment
{
    public:
        TokenSegment();
        ~TokenSegment();
        std::vector<Token>& getList(){ return tokens; }
        void push_back(TokenType,std::string);
        int size(){ return tokens.size(); }
        Token at(int loc) { return tokens.at(loc); }
        std::string getStringValue()
        {
            std::string ret;
            for(auto t : tokens)
            {
                ret += t.getValue();
            }
        }
        Token nthTokenOf(TokenType t, unsigned int i)
        {
            int inc = 0;
            for(auto ti : tokens)
            {
                if(ti.getType()==t) inc++;
                if(inc==i) return ti;
            }
            fail("Internal error: Token not found!");
            return tokens[0]; //I have to put this there to avoid compiler message
        }
        bool tokenSequencePresent(std::initializer_list<TokenType> list)
        {
            std::vector<Token>::iterator iter = tokens.begin();
            for(auto l : list)
            {
                while (iter->getType() != l) {
                iter++; if(iter->getType() == l) continue;
                if(iter->getType()==LTERM) return false;
                }
            }
            return true;
        }
        TokenSegment getLine(int linenumber) 
        {
            std::vector<Token>::iterator iter = tokens.begin();
            while(linenumber>0)
            {
                iter++;
                if(iter->getType() == LTERM) linenumber--;
            }
            iter++;
            TokenSegment ret;
            //TOWORK ON: FIX ITER POINTER, SETUP LEXER TEST
            while(iter->getType() != LTERM)
            {
                ret.append(*iter);
                iter++;
            }
            return ret;
        }
        int countAmountOfTokensBetweenTermination(TokenType type, int termnumber);
        Token find(TokenType t){std::vector<Token>::iterator iter = tokens.begin();while(iter->getType()!=t){ iter++; if(iter==tokens.end()) fail("Could not locate token"); } return *iter;}
        /*
        TokenSegment subToken(Token begin, Token end)
        {
            TokenSegment ret;
            if(std::find(tokens.begin(),tokens.end(),begin)==tokens.end() || std::find(tokens.begin(),tokens.end(),begin)==tokens.end()) //BAD CODE! FIX NOW!
            {
                fail("Could not make sub-token");
            }
            std::vector<Token>::iterator iter = std::find(tokens.begin(),tokens.end(),begin);
            unsigned int distance = std::find(tokens.begin(),tokens.end(),end) - std::find(tokens.begin(),tokens.end(),begin);
            while(distance>0)
            {
                distance--;
                ret.append(*iter);
                iter++;
            }
            return ret;
        }
        */
        void append(Token t){ tokens.push_back(t); }
    protected:
    private:
    std::vector<Token> tokens;
};

#endif // TOKENSEGMENT_H
