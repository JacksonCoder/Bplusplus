#include "TokenLexer.h"

TokenLexer::TokenLexer()
{

}

TokenLexer::~TokenLexer()
{
    //dtor
}
bool alphanum(char c)
{
    std::string letternum = "abcdefghijklmnopqrstuvwxyz01234567890_";
    if(letternum.find(c)==std::string::npos) //OPTIMIZE
    {
        return false;
    }
    return true;
}
void TokenLexer::construct(std::string name)
{
    File in;
    in.read(name);
    std::string code = in.getContents();
    std::string::iterator iter = code.begin();
    unsigned int scopelevel = 0;
    bool inFunction = false,comment=false;
    bool loop = false;
    unsigned int levels = 0;
    for(int i = 0; i < code.length();i++)
    {
        char c = code[i];
        if(comment && c != '\n'){continue;}
        else comment = false;
        if(isdigit(c))
        {
            std::string currentBuffer;
            while(isdigit(code[i])){ currentBuffer += code[i]; i++; }
            out.push_back(NUMBER,currentBuffer,scopelevel);
            i--;
            continue;
        }
        if(alphanum(c))
        {
            std::string currentBuffer;
            while(alphanum(code[i])){ currentBuffer += code[i]; i++; }
            out.push_back(TEXT,currentBuffer,scopelevel);
            i--;
            continue;
        }
        if(c=='\t' && (out.at(out.size()-1).getType()== TAB || out.at(out.size()-1).getType()==TERM))
        {
            scopelevel++;
            continue;
        }
        if(c == '\t' || c == ' ')
        {
            continue;    
        }
        if(c=='\n')
        {
            scopelevel = 0;
            if(!loop) out.push_back(TERM,"\n",scopelevel);
            else out.push_back(LOOPLINE,"\n",scopelevel);
            continue;
        }
        if(c == '(')
        {
            out.push_back(OPAREN,"(",scopelevel);
            continue;
        }
        if(c == ')')
        {
            out.push_back(CPAREN,")",scopelevel);
            continue;
        }

        if(c == ':')
        {
            out.push_back(COLON,":",scopelevel);
            continue;
        }
        if(c == ',')
        {
            out.push_back(COMMA,",",scopelevel);
            continue;
        }
        if(c == '/' && code[i-1] == '/')
        {
            comment = true;
            continue;
        }
        if(c == '\"')
        {  
            std::string currentBuffer = "\"";
            i++;
            while((code[i]!='"' || code[i-1]=='\\') && i < code.length()){currentBuffer += code[i];i++;}
            if(!(code[i]=='\"')) fail("Tokenizer Error: Unclosed quote.");
            currentBuffer += "\"";
            out.push_back(QUOTE,currentBuffer,scopelevel);
            currentBuffer.clear();
            continue;
        }
        if(c == '=')
        {
            out.push_back(EQUALS,"=",scopelevel);
            continue;
        }
    }
    for(int i = 0;i<out.size();i++) //begin lexing
    {
        if(out.tokens[i].getType() == TEXT)
        {
            Token& mod = out.tokens[i];
            //do typecheck
            if(mod.getValue()=="if")
            {
                mod.setType(IFKEYWORD);
            }
            if(mod.getValue()=="else")
            {
                mod.setType(ELSEKEYWORD);
            }
            if(mod.getValue() == "end")
            {
                mod.setType(ENDKEYWORD);
            }
        }
        //lexing done
    }
    //enforce(out);
}
