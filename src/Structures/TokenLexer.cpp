#include "TokenLexer.h"

TokenLexer::TokenLexer()
{
    //constructor
}

TokenLexer::~TokenLexer()
{
    //dtor
}

void TokenLexer::construct(std::string name)
{
    File in;
    in.read(name);
    std::string code = in.getContents();
    std::string currentBuffer,exText;
    std::string::iterator iter = code.begin();
    for(auto c : code)
    {
        std::cout<<"i"<<c<<std::endl;
        if((c ==' ')) continue;
        currentBuffer += c;
        //for all tokens
        if(currentBuffer.find("\n") != std::string::npos)
        {
           exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText);
            out.push_back(LTERM,"\n");
            currentBuffer.clear();
            continue;
        }
        if(currentBuffer.find("(") != std::string::npos)
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText);
            out.push_back(OPAREN,"(");
            currentBuffer.clear();
            continue;
        }
        if(currentBuffer.find(")") != std::string::npos)
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText);
            out.push_back(CPAREN,")");
            currentBuffer.clear();
            continue;
        }
        if(currentBuffer.find("\t") != std::string::npos && (out.at(out.size()-1).getType()== TAB || out.at(out.size()-1).getType()==LTERM))
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText);
            out.push_back(INDENT,"\t");
            currentBuffer.clear();
            continue;
        }
        if(currentBuffer.find(":") != std::string::npos)
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText);
            out.push_back(COLON,":");
            currentBuffer.clear();
            continue;
        }
        if(currentBuffer.find(",") != std::string::npos)
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText);
            out.push_back(COMMA,",");
            currentBuffer.clear();
            continue;
        }
    }
    out.push_back(LTERM,"\n");
}
