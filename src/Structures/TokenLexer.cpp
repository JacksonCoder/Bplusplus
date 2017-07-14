#include "../../Include/TokenLexer.h"

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
    std::cout << (std::find ( code.begin(), code.end(), '\t') != code.end());
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
            out.push_back(Token(NUMBER,currentBuffer,0));
            i--;
            continue;
        }
        if(alphanum(c))
        {
            std::string currentBuffer;
            while(alphanum(code[i])){ currentBuffer += code[i]; i++; }
            out.push_back(Token(TEXT,currentBuffer,0));
            i--;
            continue;
        }
        if(c == ' ')
        {
            continue;
        }
        if(c == '\t')
        {
            std::cout<< "Creating tab" << std::endl;
            out.push_back(Token(TAB,"\t",0));
            continue;
        }
        if(c=='\n')
        {
            scopelevel = 0;
            out.push_back(Token(TERM,"\n",0));
            continue;
        }
        if(c == '(')
        {
            out.push_back(Token(OPAREN,"(",0));
            continue;
        }
        if(c == ')')
        {
            out.push_back(Token(CPAREN,")",0));
            continue;
        }

        if(c == ':')
        {
            out.push_back(Token(COLON,":",0));
            continue;
        }
        if(c == ',')
        {
            out.push_back(Token(COMMA,",",0));
            continue;
        }
        if(c == '/' && code[i+1] == '/')
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
            out.push_back(Token(QUOTE,currentBuffer,0));
            currentBuffer.clear();
            continue;
        }
        if(c == '=' && code[i+1] == '=')
        {
            out.push_back(Token(EQUALS,"==",0));
            continue;
        }
        if(c == '!' && code[i+1] == '=')
        {
            out.push_back(Token(NOTEQUALS,"!=",0));
            continue;
        }
        /*
        if(c == '=')
        {
            out.push_back(Token(EQUALS,"=",0));
            continue;
        }
        */
        if(c == '@')
        {
            out.push_back(Token(AT,"@",0));
            continue;
        }
        if(c == '+')
        {
            out.push_back(Token(ADD,"+", 0)); //NEXT
            continue;
        }
        if(c == '-')
        {
            out.push_back(Token(SUBTRACT,"-",0));
            continue;
        }
        if(c == '*')
        {
            out.push_back(Token(POINTER,"*",0));
            continue;
        }
        if(c == '/')
        {
            out.push_back(Token(DIVIDE,"/",0));
            continue;
        }
        if(c == '%')
        {
            out.push_back(Token(MODULO,"%",0));
            continue;
        }
    }
    bool done = false;
    unsigned int tabindents = 0;
    while(!done)
    {
    for(int a = 0; a < 2; a++)
    {
    for(int i = 0;i<out.size();i++) //begin lexing
    {
        Token& mod = out.at(i);

        if(mod.getType() == TERM)
        {
            if(out.at(i-1).getType() == TERM)
            {
                //blank line
                out.erase(out.front()+i);
            }
            mod.setScope(tabindents);
            tabindents = 0;
            if(i + 1 >= out.size()) continue;
            for (int iter = i + 1; out.at(iter).getType() == TAB && iter < out.size(); iter++) { tabindents++;}
            continue;
        }
        if(mod.getType() == TEXT)
        {
            //do typecheck
            if(mod.getValue()=="if")
            {
                mod.setType(IFKEYWORD);
            }
            if(mod.getValue()=="else")
            {
                mod.setType(ELSEKEYWORD);
            }
            if(mod.getValue()=="while")
            {
                mod.setType(WHILEKEYWORD);
            }
            if(mod.getValue()=="foreach")
            {
                mod.setType(FOREACHKEYWORD);
            }
            if(mod.getValue()=="switch")
            {
                mod.setType(SWITCHKEYWORD);
            }
            if(mod.getValue() == "return")
            {
                mod.setType(RETURNKEYWORD);
            }
            if(mod.getValue() == "for")
            {
                mod.setType(FORKEYWORD);
            }
            if(mod.getValue() == "pointer")
            {
                mod.setType(PTRKEYWORD);
            }
            if(mod.getValue() == "reference")
            {
                mod.setType(REFKEYWORD);
            }
            if(mod.getValue() == "async")
            {
                mod.setType(ASYNCKEYWORD);
            }
            if(mod.getValue() == "const")
            {
                mod.setType(CONSTKEYWORD);
            }
            if(mod.getValue() == "safe")
            {
                mod.setType(SAFEKEYWORD);
            }
        }
        mod.setScope(tabindents);
    }
    }
    done = true;

    }
    if(tabindents != 0) fail("Tokenizer Error: Non-closed scope!");
    out.tokens.erase(std::remove_if(
    out.tokens.begin(), out.tokens.end(),
    [](Token t) {
        return t.getType()==TAB;
    }), out.tokens.end());
    //enforce(out);
}
