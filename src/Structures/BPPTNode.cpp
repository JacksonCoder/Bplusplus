#include "BPPTNode.h"

void BPPTNode::setTokenMatches(std::smatch match)
{
    this->tokenmatches.assign(match.begin(),match.end());
}
bool BPPTNode::isArglist(std::string inputToken)
{
    std::regex arglist("([^\\(\\)]+),([^\\(\\)]+)");
    if(std::regex_search(inputToken,arglist)){
        return true;

    }
    return false;
}

bool BPPTNode::isFunction(std::string inputToken){
    std::smatch matches;
    std::regex functionindentifier("\\s?(\\w+)\\(\\)");
    if(std::regex_match(inputToken,matches,functionindentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool BPPTNode::isFunctionA(std::string inputToken)
{
    std::smatch matches;
    std::regex functionaindentifier("\\s?(\\w+)\\((.+)\\)");
    if(std::regex_match(inputToken,matches,functionaindentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool BPPTNode::isFunctionT(std::string inputToken)
{
    std::regex functiontindentifier("\\s?(\\w+)\\s?\\(\\)\\s?:\\s?(.+)");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,functiontindentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool BPPTNode::isFunctionAT(std::string inputToken)
{
    std::regex functionatindentifier("\\s?(\\w+)\\s?\\(([\\w\\s:]+)\\)\\s?:\\s?(.+)");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,functionatindentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool BPPTNode::isVarInit(std::string inputToken)
{
    std::regex varinit("^(\\w+):(\\w+)$");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,varinit))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool BPPTNode::isVarDecl(std::string inputToken)
{
    std::regex vardeclaration("\\s?(\\w+):(\\w+)\\s?=\\s?(.+)");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,vardeclaration))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool BPPTNode::isImportStatement(std::string inputToken)
{
    std::regex importnidentifier("^\\s?import\\s?native\\s?(.+)$");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,importnidentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool BPPTNode::isReturnStatement(std::string inputToken)
{
    std::regex returnidentifier("^\\s?return(.+)$");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,returnidentifier))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
bool BPPTNode::isVarInitWithArguments(std::string inputToken)
{

    std::regex varinita("^\\s?([\\w\\s]+)\\s(\\w+:\\w+)$");
    std::smatch matches;
    if(std::regex_match(inputToken,matches,varinita))
    {
        setTokenMatches(matches);
        return true;
    }
    return false;
}
Type BPPTNode::determineType(std::string inputToken)
{
    if(isFunctionAT(inputToken))
    {
        return FUNCTIONAT;
    }
    if(isFunctionT(inputToken))
    {
        return FUNCTIONT;
    }

    if(isFunctionA(inputToken))
    {
        return FUNCTIONA;
    }

    if(isFunction(inputToken))
    {
        return FUNCTION;
    }
    if(isVarInitWithArguments(inputToken)){
        return VARINITA;
    }
    if(isArglist(inputToken))
    {
        return ARGLIST;
    }
    if(isVarDecl(inputToken))
    {
        return VARDEC;
    }

    if(isVarInit(inputToken))
    {
        return VARINIT;
    }
    if(isReturnStatement(inputToken))
    {
        return RETURN;
    }
    if(isImportStatement(inputToken))
    {
        return IMPORTN;
    }

    return TEXT;
}

BPPTNode::BPPTNode(Type t) : tokenmatches()
{
    type = t;
}

void BPPTNode::assembleSubNodes(BPPTokenTree& tree)
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
                branches.push_back(new BPPTNode(t,this));
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
        this->metaData["type"] = new BPPTNode("void",this);
        this->data["name"] = new BPPTNode(tokenmatches[1],TEXT); //ADDTO
        for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ BPPTNode* b = it->second; b->assembleSubNodes(tree); }
         for(int branches_i = 0; branches_i < branches.size(); branches_i ++ )
            {
                branches[branches_i]->assembleSubNodes(tree);
            }
        break;
        }
    case FUNCTIONA:
        {
        this->metaData["type"] = new BPPTNode("void",this);
        this->data["name"] = new BPPTNode(tokenmatches[1],TEXT); //2 ADDTO
        this->data["arglist"] = new BPPTNode(tokenmatches[2],ARGLIST);
        for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ BPPTNode* b = it->second; b->assembleSubNodes(tree); }
         for(int branches_i = 0; branches_i < branches.size(); branches_i ++ )
            {
                branches[branches_i]->assembleSubNodes(tree);
            }
        break;
        }
    case FUNCTIONT:
        {
        this->data["type"] = new BPPTNode(tokenmatches[2],TEXT); //2 ADDTO
        this->data["name"] = new BPPTNode(tokenmatches[1],TEXT);
        for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ BPPTNode* b = it->second; b->assembleSubNodes(tree); }
         for(int branches_i = 0; branches_i < branches.size(); branches_i ++ )
            {
                branches[branches_i]->assembleSubNodes(tree);
            }
        break;
        }
    case FUNCTIONAT:
        {
        this->data["type"] = new BPPTNode(tokenmatches[3],TEXT); //3 ADDTO
        this->data["name"] = new BPPTNode(tokenmatches[1],TEXT);
        std::cout<<"Making arglist"<<std::endl;
        this->data["arglist"] = new BPPTNode(tokenmatches[2],ARGLIST);
        for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it)
        {
            BPPTNode* b = it->second;
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
            this->data["type"] = new BPPTNode(tokenmatches[2],TEXT); //2 ADDTO
            this->data["name"] = new BPPTNode(tokenmatches[1],TEXT);
            //tree.varAdd(tokenmatches[1],tokenmatches[2],parent);
            break;
        }
    case VARINITA:
        {
            this->data["args"] = new BPPTNode(tokenmatches[1],TEXT);
            this->data["variable"] = new BPPTNode(tokenmatches[2],VARINIT);
            for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it)
            {
            BPPTNode* b = it->second;
            b->assembleSubNodes(tree);
            }
            break;
        }
    case VARDEC:
        {
        this->data["type"] = new BPPTNode(tokenmatches[2],TEXT); //3 ADDTO
        this->data["name"] = new BPPTNode(tokenmatches[1],TEXT);
        this->data["value"] = new BPPTNode(tokenmatches[3],TEXT);
        tree.varAdd(tokenmatches[1],tokenmatches[2],parent);
        //this->parent->variables.push_back(BPPTVar(tokenmatches[1],tokenmatches[2])); //put in the scope
        break;
        }
    case RETURN:
        {
            //save what value is being returned: you can use this later for compile-time features
            this->data["value"] = new BPPTNode(tokenmatches[1],TEXT);
            if(tree.varSearch(this->data["value"]->getToken(),parent))
            {
            this->data["returntype"] = new BPPTNode(tree.type(this->data["value"]->getToken(),parent),TEXT); //ADDTO
            }
            else{
                fail("Error... the variable specified to be returned was not previously declared!"); //how can we get it to match a specific line?
            }
            break;
        }
    case IMPORTN:
        {
            this->data["importvalue"] = new BPPTNode(tokenmatches[1],this);
            break;
        }
    }

}
BPPTNode::BPPTNode(std::string token,BPPTNode* parent) : token(token), tokenmatches(), parent(parent)
{
    type = this->determineType(this->token);
}

BPPTNode::BPPTNode(std::string token) : token(token), tokenmatches()
{
    type = this->determineType(this->token);
}

BPPTNode::~BPPTNode()
{
    for(int branch_i = 0; branch_i < branches.size();branch_i++)
    {
        delete branches[branch_i];
    }
}

/*
BPPError BPPTNode::appendToBranch(Type t)
{
    BPPTNode* newnode = new BPPTNode(t);
    this->branches.push_back(newnode);
}

BPPTNode* BPPTNode::findBranch(Type t)
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

BPPError BPPTNode::assemble()
{
    BPPError assembleError;
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
        for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ BPPTNode* b = it->second; b->assemble(); }
        this->result = this->data["type"]->getResult() + ' ' + this->data["name"]->getResult();
        break;
    }

    case VARINITA:
    {
        std::cout<<"Starting to compile..."<<std::endl;
        for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ BPPTNode* b = it->second; b->assemble(); }
        std::cout<<this->data["variable"]->getResult()<<std::endl;
        this->result = this->data["args"]->getResult() + " " + this->data["variable"]->getResult();
        break;
    }

    case IMPORTN:
    {
        for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ BPPTNode* b = it->second; b->assemble(); }
        this->result = "#include <" + this->data["importvalue"]->getResult() + '>';
        break;
    }

    case RETURN:
        {
           for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ BPPTNode* b = it->second; b->assemble(); }
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
            for(std::map<std::string,BPPTNode*>::iterator it=data.begin(); it!=data.end(); ++it){ BPPTNode* b = it->second; b->assemble(); }
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
                BPPTNode* b = branches[i];
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
                BPPTNode* b = branches[i];
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
                BPPTNode* b = branches[i];
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
                 BPPTNode* b = branches[i];
                 b->assemble();
                 result += b->getResult() + ";\n";
            }
            result += "\n}";
            break;
    }

    return assembleError;
}
