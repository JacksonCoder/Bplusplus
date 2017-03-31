#include "ASTNode.h"
/*
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
*/
ASTNode::ASTNode(Type t): type(t)
{}
ASTNode* ASTNode::assembleTop(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(ROOT);
    bool processing = true;
    while(processing)
    {
    std::vector<Token> line;
    std::vector<Token>::iterator line_delimiter_iter = ts.tokens.begin();
    while(line_delimiter_iter->getType()!=TERM) 
    { 
        line.push_back(*line_delimiter_iter); 
        line_delimiter_iter++; 
        if(ASTNode::checkIdentification(line,FUNCTIONHEAD)){
            while(line_delimiter_iter->getType()!=FUNCTIONEND)
            {
                line_delimiter_iter++;
                line.push_back(*line_delimiter_iter);
            }
            return_node->branches.push_back(assembleFunction(TokenSegment(line)));
            continue;
        } 
    }
    return_node->branches.push_back(assembleCmd(TokenSegment(line)));
    }
    return return_node;
}

ASTNode* ASTNode::assembleFunc(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(FUNCTION);
    //assemble the top first
    std::vector<Token> top;
    std::vector<Token> body;
    std::vector<Token>::iterator delimiter_iter = ts.tokens.begin();
    while(line_delimiter_iter->getType()!=TERM)
    {
        delimiter_iter++;
        top.push_back(*delimiter_iter);
    }
    return_node->branches.push_back(ASTNode::assembleFunctionHeader(TokenSegment(top)));
    while(delimiter_iter!=ts.tokens.end()) //<- does it compare classes if you have a iterator?
    {
        delimiter_iter++;
        body.push_back(*delimiter_iter);
    }
    return_node->branches.push_back(assembleCmdSeq(body));
    //Notes:
    // Make sure that all low-level data is brought to the root for the final process
    return return_node;
}
ASTNode* ASTNode::assembleFunctionHeader(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(FUNCTIONHEAD);
    
        //both args and type
        return_node->data["name"] = new ASTNode(FUNCNAME); //<- No further work needs to be done: it is just a pure string
        return_node->data["name"]->setToken(ts.nthTokenOf(NAME,1).getValue()); //<- put this line and previous into other function?
        return_node->data["args"] = ASTNode::assembleVarList(ts.getBetween(OPAREN,CPAREN)); //TokenSegment should handle this
    
    return return_node;
}
static ASTNode* ASTNode::assembleCmdSeq(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(CMDSEQ);
    std::vector<Token>::iterator iter;
    while(iter->getType != TOKENEND)
    {
        TokenSegment cmd;
        while(iter->getType() != TERM){ iter++; cmd.push_back(*iter); }
        return_node->branches.push_back(ASTNode::assembleCmd(cmd));
    }
    return return_node;
}
ASTNode* ASTNode::assembleCmd(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(CMD);
    //find type of command: single line, or loop?
    if(checkIdentification(cmd,LOOP))
    {
        return_node->branches.push_back(ASTNode::assembleLoop(ts));
    }
    else
    {
        if(ASTNode::checkIdentification(ts,VARDEC)) return_node->branches.push_back(ASTNode::assembleVariableDeclaration(ts));
        else if(ASTNode::checkIdentification(ts,VARSET)) return_node->branches.push_back(ASTNode::assembleVariableAssignment(ts));
        //else if(ASTNode::checkIdentification(ts,VARDEC)) return_node->branches.push_back(ASTNode::assembleVariableDeclaration(ts));
    }
    return return_node;
}
bool ASTNode::checkIdentification(TokenSegment ts,Type t)
{
    switch(t){
        case LOOP:
        {
            if(ts.nthTokenOf(IFKEYWORD,1)) 
            {
                return true;
            }
            return false;
        }
        case FUNCTIONHEAD:
        {
            return false;   
        }
    }
}
/*
ASTNode* ASTNode::assembleLoop(TokenSegment ts)
{
    
}
/*
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
        this->metaData["type"] = new ASTNode(TokenSegment(Token(VOIDTYPE,"void")),this);
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
        this->metaData["type"] = new ASTNode(TokenSegment(Token(VOIDTYPE,"void")),this);
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
*/
ASTNode::~ASTNode()
{
    for(int branch_i = 0; branch_i < branches.size();branch_i++)
    {
        delete branches[branch_i];
    }
}

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
