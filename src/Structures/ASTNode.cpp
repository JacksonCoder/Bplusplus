#include "ASTNode.h"

ASTNode::ASTNode(Type t) : tokenmatches()
{
    type = t;
}

ASTNode::ASTNode(TokenSegment ts,ASTNode* parent) : tokenseg(ts), tokenmatches(), parent(parent)
{
    token = ts.getStringValue();
    type = this->determineType(this->tokenseg);
}

ASTNode::ASTNode(TokenSegment ts) : tokenseg(token), tokenmatches()
{
    token = ts.getStringValue();
    type = this->determineType(this->tokenseg);
}

ASTNode(std::string s ,ASTNode* parent) : token(s), tokenmatches(), parent(parent)
{
    
}
ASTNode(std::string s) : token(s), tokenmatches()
{
    
}
void ASTNode::setTokenMatches(std::smatch match)
{
    this->tokenmatches.assign(match.begin(),match.end());
}
bool ASTNode::isArglist(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue(); //do this for everything
    
    std::regex arglist("([^\\(\\)]+),([^\\(\\)]+)");
    if(std::regex_search(inputToken,arglist)){
        return true;

    }
    return false;
}

bool ASTNode::isFunction(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue();
    std::smatch matches;
    std::regex functionindentifier("\\s?(\\w+)\\(\\)");
    if(std::regex_match(inputToken,matches,functionindentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool ASTNode::isFunctionA(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue();
    std::smatch matches;
    std::regex functionaindentifier("\\s?(\\w+)\\((.+)\\)");
    if(std::regex_match(inputToken,matches,functionaindentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool ASTNode::isFunctionT(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue();
    std::regex functiontindentifier("\\s?(\\w+)\\s?\\(\\)\\s?:\\s?(.+)");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,functiontindentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool ASTNode::isFunctionAT(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue();
    std::regex functionatindentifier("\\s?(\\w+)\\s?\\(([\\w\\s:]+)\\)\\s?:\\s?(\\w+)");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,functionatindentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    
    return false;
}
bool ASTNode::isVarInit(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue();
    std::regex varinit("^(\\w+):(\\w+)$");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,varinit))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool ASTNode::isVarDecl(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue();
    std::regex vardeclaration("\\s?(\\w+):(\\w+)\\s?=\\s?(.+)");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,vardeclaration))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool ASTNode::isImportStatement(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue();
    std::regex importnidentifier("^\\s?import\\s?native\\s?(.+)$");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,importnidentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool ASTNode::isReturnStatement(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue();
    std::regex returnidentifier("^\\s?return(.+)$");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,returnidentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool ASTNode::isVarInitWithArguments(TokenSegment ts)
{
    std::string inputToken = ts.getStringValue();
    std::regex varinita("^\\s?([\\w\\s]+)\\s(\\w+:\\w+)$");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,varinita))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
Type ASTNode::determineType(TokenSegment ts)
{
    if(isFunctionAT(ts))
    {
        return FUNCTIONAT;
    }
    if(isFunctionT(ts))
    {
        return FUNCTIONT;
    }

    if(isFunctionA(ts))
    {
        return FUNCTIONA;
    }

    if(isFunction(ts))
    {
        return FUNCTION;
    }
    if(isVarInitWithArguments(ts)){
        return VARINITA;
    }
    if(isArglist(ts))
    {
        return ARGLIST;
    }
    if(isVarDecl(ts))
    {
        return VARDEC;
    }

    if(isVarInit(ts))
    {
        return VARINIT;
    }
    if(isReturnStatement(ts))
    {
        return RETURN;
    }
    if(isImportStatement(ts))
    {
        return IMPORTN;
    }
    return TEXT;
}

void ASTNode::assembleSubNodes(ASTTree& tree)
{
    determineType(this->getToken());
    switch(type)
    {
    case ARGLIST:
        {

            std::string s = token;
            size_t pos = 0;
            std::string local_token;
            tokenmatches.clear();
            pos = s.find(",");
            while (pos != std::string::npos)
            {
                local_token = s.substr(0, pos);
                tokenmatches.push_back(local_token);
                s.erase(0, pos + 1);
            }
        local_token = s.substr(0,s.length());
        tokenmatches.push_back(token);
            for(auto t : tokenmatches)
            {
                branches.push_back(new ASTNode(t,this));
            }
            for(int branches_i = 0; branches_i < branches.size(); branches_i ++ )
            {
                branches[branches_i]->assembleSubNodes(tree);
            }
            break;
        }
        case ROOT:
        {
             for(int branches_i = 0; branches_i < branches.size(); branches_i ++ )
            {
                branches[branches_i]->assembleSubNodes(tree);
            }

            break;
        }
    case FUNCTION:
        {
        this->metaData["type"] = new ASTNode("void",this);
        this->data["name"] = new ASTNode(tokenmatches[1],TEXT); //ADDTO
        for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ ASTNode* b = it->second; b->assembleSubNodes(tree); }
         for(int branches_i = 0; branches_i < branches.size(); branches_i ++ )
            {
                branches[branches_i]->assembleSubNodes(tree);
            }
        break;
        }
    case FUNCTIONA:
        {
        this->metaData["type"] = new ASTNode("void",this);
        this->data["name"] = new ASTNode(tokenmatches[1],TEXT); //2 ADDTO
        this->data["arglist"] = new ASTNode(tokenmatches[2],ARGLIST);
        for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ ASTNode* b = it->second; b->assembleSubNodes(tree); }
         for(int branches_i = 0; branches_i < branches.size(); branches_i ++ )
            {
                branches[branches_i]->assembleSubNodes(tree);
            }
        break;
        }
    case FUNCTIONT:
        {
        this->data["type"] = new ASTNode(tokenmatches[2],TEXT); //2 ADDTO
        this->data["name"] = new ASTNode(tokenmatches[1],TEXT);
        for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ ASTNode* b = it->second; b->assembleSubNodes(tree); }
         for(int branches_i = 0; branches_i < branches.size(); branches_i ++ )
            {
                branches[branches_i]->assembleSubNodes(tree);
            }
        break;
        }
    case FUNCTIONAT:
        {
        this->data["type"] = new ASTNode(tokenmatches[3],TEXT); //3 ADDTO
        this->data["name"] = new ASTNode(tokenmatches[1],TEXT);
        std::cout<<"Making arglist"<<std::endl;
        this->data["arglist"] = new ASTNode(tokenmatches[2],ARGLIST);
        for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it)
        {
            ASTNode* b = it->second;
            b->assembleSubNodes(tree);
        }
         for(int branches_i = 0; branches_i < branches.size(); branches_i ++ )
            {
                branches[branches_i]->assembleSubNodes(tree);
            }
        break;
        }
    case VARINIT:
        {
            std::cout<<tokenmatches[1]<<std::endl;
            this->data["type"] = new ASTNode(tokenmatches[2],TEXT); //2 ADDTO
            this->data["name"] = new ASTNode(tokenmatches[1],TEXT);
            //tree.varAdd(tokenmatches[1],tokenmatches[2],parent);
            break;
        }
    case VARINITA:
        {
            this->data["args"] = new ASTNode(tokenmatches[1],TEXT);
            this->data["variable"] = new ASTNode(tokenmatches[2],VARINIT);
            for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it)
            {
            ASTNode* b = it->second;
            b->assembleSubNodes(tree);
            }
            break;
        }
    case VARDEC:
        {
        this->data["type"] = new ASTNode(tokenmatches[2],TEXT); //3 ADDTO
        this->data["name"] = new ASTNode(tokenmatches[1],TEXT);
        this->data["value"] = new ASTNode(tokenmatches[3],TEXT);
        tree.varAdd(tokenmatches[1],tokenmatches[2],parent);
        //this->parent->variables.push_back(TVar(tokenmatches[1],tokenmatches[2])); //put in the scope
        break;
        }
    case RETURN:
        {
            //save what value is being returned: you can use this later for compile-time features
            this->data["value"] = new ASTNode(tokenmatches[1]);
            //try to get the value of this expression
            if(tree.varSearch(this->data["value"]->getToken(),parent)) //pure variable
            {
            this->data["returntype"] = new ASTNode(tree.type(this->data["value"]->getToken(),parent),TEXT); //ADDTO
            }
            else{
                fail("Error... the variable specified to be returned was not previously declared!"); //how can we get it to match a specific line?
            }
            break;
        }
    case IMPORTN:
        {
            this->data["importvalue"] = new ASTNode(tokenmatches[1],this);
            break;
        }
    }

}

ASTNode::~ASTNode()
{
    for(int branch_i = 0; branch_i < branches.size();branch_i++)
    {
        delete branches[branch_i];
    }
}

/*
Error ASTNode::appendToBranch(Type t)
{
    ASTNode* newnode = new ASTNode(t);
    this->branches.push_back(newnode);
}

ASTNode* ASTNode::findBranch(Type t)
{
    for(int vect_i = 0; vect_i < branches.size();vect_i++)
    {
        if(branches[vect_i]->getType() == t){
            return branches[vect_i];
        }
    }
    return nullptr;
}
*/

void ASTNode::assemble()
{
    switch(type) {

case ARGLIST:
    {
        for(auto b : branches)
        {
            std::cout<<b->getType()<<std::endl;
            b->assemble();
            this->result += b->getResult() + ',';
        }
        result.erase(result.end()-1, result.end());
        break;
    }

    case VARINIT:
    {
        for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ ASTNode* b = it->second; b->assemble(); }
        this->result = this->data["type"]->getResult() + ' ' + this->data["name"]->getResult();
        break;
    }

    case VARINITA:
    {
        std::cout<<"Starting to compile..."<<std::endl;
        for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ ASTNode* b = it->second; b->assemble(); }
        std::cout<<this->data["variable"]->getResult()<<std::endl;
        this->result = this->data["args"]->getResult() + " " + this->data["variable"]->getResult();
        break;
    }

    case IMPORTN:
    {
        for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ ASTNode* b = it->second; b->assemble(); }
        this->result = "#include <" + this->data["importvalue"]->getResult() + '>';
        break;
    }

    case RETURN:
        {
           for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ ASTNode* b = it->second; b->assemble(); }
           this->result = "return " + this->data["value"]->getResult();
           break;
        }

    case TEXT:
    {
        this->result = this->token;
        break;
    }

    case ROOT:
        {
            for(auto b : branches)
            {
                b->assemble();
            }
            for(auto b : branches)
            {
                this->result += b->result + '\n';
            }
            break;
        }

    case BLANKLINE:
        {
            this->result = "";
            break;
        }

    case VARDEC:
        {
            for(std::map<std::string,ASTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ ASTNode* b = it->second; b->assemble(); }
            this->result = this->data["type"]->getResult() + ' ' + this->data["name"]->getResult() + " = " + this->data["value"]->getResult();
            break;
        }

        case FUNCTIONAT:
            {
            if(data.find("name") == data.end() || data.find("type") == data.end() || data.find("arglist") == data.end())
                {
                    assembleError.setMessage("Error while assembling file: broken tree");
                    assembleError.setError(true);
                    break;
                }
            result = "";
            data["arglist"]->assemble();
            std::string argslistResult = data["arglist"]->getResult();
            result += data["type"]->getToken() + ' ' + data["name"]->getToken();
            result += '(' + argslistResult + ')';
            result += "{\n";
            for(int i = 0; i < branches.size(); i ++)
            {
                ASTNode* b = branches[i];
                 b->assemble();
                 result += b->getResult() + ";\n";
            }
            result += "\n}";
            break;
            }

        case FUNCTIONT:
            {

            if(data.find("name") == data.end() || data.find("type") == data.end())
                {
                    assembleError.setMessage("Error while assembling file: broken tree");
                    assembleError.setError(true);
                    break;
                }
            result = "";
            result += data["type"]->getToken() + ' ' + data["name"]->getToken() + "()";
            result += "{\n";
            for(int i = 0; i < branches.size(); i ++)
            {
                ASTNode* b = branches[i];
                 b->assemble();
                 result += b->getResult() + ";\n";
            }
            result += "\n}";
            break;
            }

        case FUNCTIONA:
            {
            if(data.find("name") == data.end() || metaData.find("type") == metaData.end() || data.find("arglist") == data.end())
                {
                    assembleError.setMessage("Error while assembling file: broken tree");
                    assembleError.setError(true);
                    break;
                }
            data["arglist"]->assemble();
            result = "";
            result += metaData["type"]->getToken() + ' ' + data["name"]->getToken();
            result += '(' + data["arglist"]->getResult() + ')';
            result += "{\n";
            for(int i = 0; i < branches.size(); i ++)
            {
                ASTNode* b = branches[i];
                 b->assemble();
                 result += b->getResult() + ";\n";
            }
            result += "\n}";
            break;
            }

        case FUNCTION:
            if(data.find("name") == data.end() || metaData.find("type") == metaData.end())
                {
                    assembleError.setMessage("Error while assembling file: broken tree");
                    assembleError.setError(true);
                    break;
                }
            result = "";
            result += metaData["type"]->getToken() + ' ' + data["name"]->getToken() + "()";
            result += "{\n";
            for(int i = 0; i < branches.size(); i ++)
                {
                 ASTNode* b = branches[i];
                 b->assemble();
                 result += b->getResult() + ";\n";
            }
            result += "\n}";
            break;
    }
}
