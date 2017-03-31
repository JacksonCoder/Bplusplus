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
    unsigned int scopelevel = 0,prevscope=0;
    bool inFunction = false;
    bool loop = false;
    for(auto c : code)
    {
        std::cout<<"c:"<<c<<std::endl;
        currentBuffer += c;
        if(currentBuffer.find("\t") != std::string::npos && (out.at(out.size()-1).getType()== TAB || out.at(out.size()-1).getType()==TERM))
        {
            scopelevel++;
            currentBuffer.erase();
            continue;
        }
        if(c ==' ' || c=='\t'){
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1); 
            if(exText.length()>0) out.push_back(NAME,exText,scopelevel); 
            out.push_back(WHITESPACE," ",scopelevel);
        }
        if(currentBuffer.find("\n") != std::string::npos)
        {
           exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText,scopelevel);
            //finish line parsing:
            if(scopelevel<prevscope)
            {
                std::cout<<"Dedent!"<<std::endl;
            }
            prevscope = scopelevel;
            scopelevel = 0;
            if(!loop) out.push_back(TERM,"\n",scopelevel);
            else out.push_back(LOOPLINE,"\n",scopelevel);
            currentBuffer.clear();
            continue;
        }
        if(currentBuffer.find("(") != std::string::npos)
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText,scopelevel);
            out.push_back(OPAREN,"(",scopelevel);
            currentBuffer.clear();
            continue;
        }
        if(currentBuffer.find(")") != std::string::npos)
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText,scopelevel);
            out.push_back(CPAREN,")",scopelevel);
            currentBuffer.clear();
            continue;
        }

        if(currentBuffer.find(":") != std::string::npos)
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText,scopelevel);
            out.push_back(COLON,":",scopelevel);
            currentBuffer.clear();
            continue;
        }
        if(currentBuffer.find(",") != std::string::npos)
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText,scopelevel);
            out.push_back(COMMA,",",scopelevel);
            currentBuffer.clear();
            continue;
        }
        if(currentBuffer.find("if") != std::string::npos)
        {
            exText .assign(currentBuffer.begin(),currentBuffer.end()-1);
            if(exText.length()>0) out.push_back(NAME,exText,scopelevel);
            out.push_back(IFKEYWORD,"if",scopelevel);
            currentBuffer.clear();
            continue;
        }
    }
    out.push_back(TERM,"\n",scopelevel);
    for(int i = 0;i<out.size();i++) //iterate to clean segment
    {
        //if(out[i].getType()==COMMENT) out.erase(i);
    }
    //enforce(out);
}
