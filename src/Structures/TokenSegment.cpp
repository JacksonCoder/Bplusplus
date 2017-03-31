#include "TokenSegment.h"

TokenSegment::TokenSegment()
{
    //ctor
}

TokenSegment::~TokenSegment()
{
    //dtor
}

void TokenSegment::push_back(TokenType t,std::string s,int i)
{
    tokens.push_back(Token(t,s,i));
}
int TokenSegment::countAmountOfTokensBetweenTermination(TokenType type, int termnumber){
            unsigned int count = 0;
            std::vector<Token>::iterator iter = tokens.begin();
            while(termnumber>0)
            {
                iter++;
                if(iter->getType() == TERM) termnumber--;
            }
            iter++;
            while(iter->getType() != TERM)
            {
                if(iter->getType() == type) count++;
                iter++;
            }
            return count;
}