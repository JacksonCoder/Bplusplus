#include "ASTNode.h"

ASTNode::ASTNode(Type t): type(t)
{}
void ASTNode::print_tree(int scope) //for debugging purposes
{
    for(int i = 0 ; i < scope;i++)
    {
        std::cout<<"-";
    }
    std::cout<<"type:"<<type<<std::endl;
    for(auto b : branches)
    {
        b->print_tree(scope+1);
    }
}
ASTNode* ASTNode::assembleTop(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(ROOT);
    bool loop = false;
    std::vector<Token> line;
    return_node = ASTNode::assembleCmdSeq(ts,false);
    return_node->print_tree(0);
    return return_node;
}

ASTNode* ASTNode::assembleFunc(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(FUNCTION);
    //assemble the top first
    std::vector<Token> top;
    std::vector<Token> body;
    std::vector<Token>::iterator delimiter_iter = ts.tokens.begin();
    while(delimiter_iter->getType()!=TERM)
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
    return_node->branches.push_back(assembleCmdSeq(body,false));
    //Notes:
    // Make sure that all low-level data is brought to the root for the final process
    return return_node;
}
ASTNode* ASTNode::assembleLoop(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(IF);
    return return_node;
}
ASTNode* ASTNode::assembleFunctionHeader(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(FUNCTIONHEAD);
    /*
        //both args and type
        return_node->data["name"] = new ASTNode(FUNCNAME); //<- No further work needs to be done: it is just a pure string
        return_node->data["name"]->setToken(ts.nthTokenOf(NAME,1).getValue()); //<- put this line and previous into other function?
        //return_node->data["args"] = ASTNode::assembleVarList(ts.getBetween(OPAREN,CPAREN)); //TokenSegment should handle this
    */
    return return_node;
}

ASTNode* ASTNode::assembleCmdSeq(TokenSegment ts,bool isLoop)
{
    //std::cout<<ts.tokens.end()->getValue()<<std::endl;
    std::cout<<"Checking command sequence "<<ts.getStringValue()<<std::endl;
    ASTNode* return_node = new ASTNode(CMDSEQ);
    unsigned int i = 0;
    while(i < ts.tokens.size())
    {
        TokenSegment cmd;
        cmd = ts.createUntil({TERM},i,ts,false);
        //if unique command detected, continue
        std::cout<<"Made command:"<<cmd.getStringValue()<<std::endl;
        TokenSegment temp;
        if(checkIdentification(cmd,IFHEADER))
        {
            std::cout<<"Identified as if statement.";
            temp = ts.createUntil({ENDKEYWORD},i,ts,true);
            i++;
        }
        for(auto t : temp.tokens)
        {
            cmd.tokens.push_back(t);
        }
        return_node->branches.push_back(ASTNode::assembleCmd(cmd));
        i++;
    }
    return return_node;
}
ASTNode* ASTNode::assembleCmd(TokenSegment ts)
{
    std::cout<<"Checking command:("<<ts.getStringValue()<<")\n";
    ASTNode* return_node;
    if(checkIdentification(ts,IFHEADER))
    {
        return_node = ASTNode::assembleIf(ts);
    }
    else if(ASTNode::checkIdentification(ts,RETURNCMD)) return_node = ASTNode::assembleReturnCmd(ts);
    //else if(ASTNode::checkIdentification(ts,VARDEC)) return_node->branches.push_back(ASTNode::assembleVariableDeclaration(ts));
    //else if(ASTNode::checkIdentification(ts,VARSET)) return_node->branches.push_back(ASTNode::assembleVariableAssignment(ts));
    //else if(ASTNode::checkIdentification(ts,VARDEC)) return_node->branches.push_back(ASTNode::assembleVariableDeclaration(ts));
    else{
        return_node = new ASTNode(CMD);
        return_node->token = ts.getStringValue();
    }
    std::cout<<"Command checked!"<<std::endl;
    return return_node;
}
ASTNode* ASTNode::assembleIf(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(IF);
    unsigned int i = 0;
    TokenSegment line;
    line = ts.createUntil({TERM},i,ts,false);
    ASTNode* head = ASTNode::assembleIfHeader(line);
    line.tokens.clear();
    i++;
    line = ts.createUntil({ENDKEYWORD},i,ts,true);
    //line.push_back(Token(ENDKEYWORD,"end",line[line.size()-1].scopenumber));
    std::cout<<"Assembling line:"<<TokenSegment(line).getStringValue()<<std::endl;
    ASTNode* body = ASTNode::assembleCmdSeq(TokenSegment(line),true);
    return_node->branches.push_back(head);
    return_node->branches.push_back(body);
    std::cout<<"Closing if"<<std::endl;
    return return_node; //NEXT add header cmds and generalize looping. FUTURE: add functions
}

ASTNode* ASTNode::assembleIfHeader(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(IFHEADER);
    std::cout<<"Assembling header!";
    return_node->data["expr"] = ASTNode::assembleCmd(ts.nthTokenOf(TEXT,1)); //CHANGE
    return return_node;
}

ASTNode* ASTNode::assembleReturnCmd(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(RETURNCMD);
    unsigned int tokenlength = ts.size() - 1; //1 for the return keyword
    if(tokenlength<=0) fail("Internal Error: 'return' statement identification flawed");
    unsigned int i = 1;
    TokenSegment expr;
    expr = ts.createUntil({TERM},i,ts,false);
    return_node->data["expr"] = ASTNode::assembleCmd(expr);
    return return_node;
}

ASTNode* ASTNode::assembleFor(TokenSegment ts)
{
    //this should have an ending statement. If a ending statement can not be found, we have a parsing problem
    ASTNode* return_node = new ASTNode(FOR);
    unsigned int i = 0;
    TokenSegment header = ts.createUntil({TERM},i,ts,false);
    ASTNode* head = ASTNode::assembleForHeader(header);
    i++;
    TokenSegment interior = ts.createUntil({ENDKEYWORD},i,ts,true);
    ASTNode* body = ASTNode::assembleCmdSeq(interior,false);
    return_node->branches.push_back(head);
    return_node->branches.push_back(body);
    std::cout<<"Closing for"<<std::endl;
    return return_node;
}

ASTNode* ASTNode::assembleForHeader(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(FORHEADER);
    return_node->token = ts.getStringValue();
    return return_node;
}
bool ASTNode::checkIdentification(TokenSegment ts,Type t)
{
    switch(t){
        case IFHEADER:
        {
            if(ts.tokens[0].getType()==IFKEYWORD && ts.size() > 1) //crude definition, but should work
            {
                return true;
            }
            return false;
        }
        case RETURNCMD:
        {
            if(ts.tokens[0].getType()==RETURNKEYWORD&& ts.size() > 1)
            {
                std::cout<<"It's a return keyword!"<<std::endl;
                return true;   
            }
            if(ts.tokens[0].getType() == RETURNKEYWORD) fail("Code error: Return statement is not finished!");
            return false;
        }
        case FORHEADER:
        {
            //NEXT
            break;
        }
        default:
        {
            return false;
        }
    }
}
/*
ASTNode* ASTNode::assembleLoop(TokenSegment ts)
{
    
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

case ROOT:
    {
        for(auto b : branches)
        {
            b->assemble();
            this->result += b->getResult();
        }
        break;
    }

    case IF:
    {
        branches[0]->assemble();
        result = "if("+branches[0]->getResult()+") {\n";
        branches[1]->assemble();
        result += branches[1]->getResult();
        result += "}";
        break;
    }
    case CMD: 
    {
        result = token;
        break;
    }
    case CMDSEQ:
    {
        std::cout<<branches.size();
        for(auto b : branches)
        {
            b->assemble();
            result += b->getResult() + ";\n";
        }
        break;
    }
    case IFHEADER:
    {
        result = data["expr"]->getToken();
        break;
    }
    case RETURNCMD:
    {
        result = "return " + data["expr"]->getToken();
        break;
    }
    case FORHEADER:
    {
        result = token;
        break;
    }
    case FOR:
    {
        for(auto b:branches)
        {
            b->assemble();
        }
        result = branches[0]->getResult() += "{\n" + branches[1]->getResult() + "}";
        break;
    }
    default:
    {
        //fail("Unsupported token!");
        break;
    }
}
}
