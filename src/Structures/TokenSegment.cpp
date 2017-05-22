#include "../../Include/TokenSegment.h"

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
    tokens.push_back(Token(t,s,0));
}
void TokenSegment::next() { currentInc++;}
        Token TokenSegment::get(){ return tokens[currentInc]; }
        TokenType TokenSegment::type(){return tokens[currentInc].getType(); }
        int TokenSegment::size(){ return tokens.size(); }
        Token TokenSegment::at(int loc) { return tokens.at(loc); }
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
        std::vector<Token> TokenSegment::createUntil(std::initializer_list<TokenType> tokentypeil,unsigned int& i,TokenSegment& ts,bool processing_loop)
        {
            std::vector<Token> ret;
            bool processing = true;
            //if(std::find(ts.tokens.begin(),ts.tokens.end(),*iter) == ts.tokens.end()) fail("Internal Error: parsing exclusively end of data!");

            std::cout<<"Parsing ("<<ts.getStringValue()<<")\n";
            unsigned int current_scope = ts.tokens[i].scopenumber;
            while(processing && i < ts.tokens.size())
            {
                for(auto tt : tokentypeil)
                {
                    if(ts.tokens[i].getType() == tt && (ts.tokens[i].scopenumber == current_scope || !processing_loop))
                    {
                        std::cout<<"Scope at:"<<ts.tokens[i].scopenumber<<std::endl;
                        processing = false;
                        std::cout<<"Stopping at "<<ts.tokens[i].getType()<<std::endl;
                    }
                }
                if(!processing) break;
                ret.push_back(ts.tokens[i]);
                std::cout<<"Iterating";
                i++;
            }
            return ret;
        }