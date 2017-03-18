#include "TokenSegment.h"

TokenSegment::TokenSegment()
{
    //ctor
}

TokenSegment::~TokenSegment()
{
    //dtor
}

void TokenSegment::push_back(TokenType t,std::string s)
{
    tokens.push_back(Token(t,s));
}
int TokenSegment::countAmountOfTokensBetweenTermination(TokenType type, int termnumber){
            unsigned int count = 0;
            std::vector<Token>::iterator iter = tokens.begin();
            while(termnumber>0)
            {
                iter++;
                if(iter->getType() == LTERM) termnumber--;
            }
            iter++;
            while(iter->getType() != LTERM)
            {
                if(iter->getType() == type) count++;
                iter++;
            }
            return count;
        }