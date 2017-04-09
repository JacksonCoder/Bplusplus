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
        TokenSegment(std::vector<Token> tokens): tokens(tokens) {}
        TokenSegment(Token t) { tokens.push_back(t); }
        void next() { currentInc++;}
        Token get(){ return tokens[currentInc]; }
        std::vector<Token>& getList(){ return tokens; }
        void push_back(TokenType,std::string,int);
        int size(){ return tokens.size(); }
        Token at(int loc) { return tokens.at(loc); }
        TokenSegment getBetween(TokenType first, TokenType second)
        {
            std::vector<Token>::iterator iter = tokens.begin();
            while(iter->getType() != first) iter++;
            TokenSegment ret;
            while(iter->getType() != second){ iter++;ret.tokens.push_back(*iter); }
            return ret;
        }
        std::string getStringValue()
        {
            std::string ret;
            for(auto t : tokens)
            {
                ret += t.getValue();
            }
            return ret;
        }
        Token nthTokenOf(TokenType t, unsigned int i)
        {
            int inc = 0;
            for(auto ti : tokens)
            {
                if(ti.getType()==t) inc++;
                if(inc==i) return ti;
            }
            Token failure(TOKENEND," ",0);
            return failure; //I have to put this there to avoid a compiler message
        }
        bool tokenSequencePresent(std::initializer_list<TokenType> list)
        {
            std::vector<Token>::iterator iter = tokens.begin();
            if(list.size()<=tokens.size()) for(auto l : list)
            {
                while (iter->getType() != l && iter->getType()!=TOKENEND) {
                
                iter++;
                if(iter->getType()==TOKENEND) return false;
                if(iter->getType() == l) continue;
                }
            }
            else return false;
            return true;
        }
        TokenSegment getLine(int linenumber) 
        {
            std::vector<Token>::iterator iter = tokens.begin();
            while(linenumber>0)
            {
                iter++;
                if(iter->getType() == TERM) linenumber--;
            }
            iter++;
            TokenSegment ret;
            //TOWORK ON: FIX ITER POINTER, SETUP LEXER TEST
            while(iter->getType() != TERM)
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
        std::vector<Token> createUntil(std::initializer_list<TokenType> tokentypeil,std::vector<Token>::iterator& iter,bool include_tokenend)
        {
            std::vector<Token> ret;
            bool processing = true;
            while(processing)
            {
                for(auto tt : tokentypeil)
                {
                    if(iter->getType() == tt)
                    {
                        processing = false;
                    }
                }
                if(!processing) break;
                ret.push_back(*iter);
                iter++;
            }
            if(include_tokenend) ret.push_back(Token(TOKENEND," ",0));
            return ret;
        }
            std::vector<Token> tokens;
    protected:
    private:
    int currentInc = 0;
    std::string cached;
};

#endif // TOKENSEGMENT_H
