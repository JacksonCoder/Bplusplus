#include "../../Include/TokenSegment.h"

TokenSegment::TokenSegment()
{
}

TokenSegment::~TokenSegment()
{
    //dtor
}

void TokenSegment::push_back(Token t)
{
    tokens.push_back(t);
}
void TokenSegment::next() { currentInc++;}
Token TokenSegment::get(){ return tokens[currentInc]; }
TokenType TokenSegment::type(){return tokens[currentInc].getType(); }
std::string TokenSegment::value(){return tokens[currentInc].getValue(); }
int TokenSegment::size(){ return tokens.size(); }
Token& TokenSegment::at(int loc) { return tokens.at(loc); }
void TokenSegment::reset() { currentInc = 0; }
void TokenSegment::erase(std::vector<Token>::iterator erasearea)
{
    tokens.erase(erasearea);
}
std::string TokenSegment::getStringValue()
{
    std::string ret;
    for(auto t : tokens)
    {
        ret += t.getValue();
    }
    return ret;
}

        void TokenSegment::append(Token t){ tokens.push_back(t); }
        TokenSegment TokenSegment::eatIndented(TokenSegment& ts)
        {
            TokenSegment ret;
            unsigned int scope = ts.at(ts.current()-1).scopenumber;
             while(ts.scope() > scope && !ts.end())
            {
                ret.push_back(ts.get());
                ts.next();
            }
            return ret;
        }
        TokenSegment TokenSegment::eatLine(TokenSegment& ts)
        {
            TokenSegment ret;
            while(ts.type() != TERM && !ts.end())
            {
                ret.push_back(ts.get());
                ts.next();
            }
            return ret;
        }