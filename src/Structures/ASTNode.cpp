#include "ASTNode.h"

ASTNode::ASTNode(Type t): type(t)
{}
void ASTNode::print_tree() //for debugging purposes
{
    std::cout<<"type:"<<type<<std::endl;
    for(auto b : branches)
    {
        b->print_tree();
    }
}
ASTNode* ASTNode::assembleTop(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(ROOT);
    bool loop = false;
    std::vector<Token> line;
    std::vector<Token>::iterator line_delimiter_iter = ts.tokens.begin();
    while(line_delimiter_iter->getType()!=TOKENEND)
    {
        std::cout<<"Creating new command!"<<std::endl;
    while(line_delimiter_iter->getType()!=TERM && line_delimiter_iter->getType()!=TOKENEND) 
    { 
        line.push_back(*line_delimiter_iter); 
        line_delimiter_iter++;
    }
    return_node->branches.push_back(assembleCmd(TokenSegment(line))); 
    line.clear();
    line_delimiter_iter++;
}   
    return_node->print_tree();
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
    std::cout<<"Checking command sequence "<<ts.getStringValue()<<std::endl;
    ASTNode* return_node = new ASTNode(CMDSEQ);
    std::vector<Token>::iterator iter = ts.tokens.begin();
    while(iter->getType() != TOKENEND)
    {
        TokenSegment cmd;
        if(!isLoop) while(iter->getType() != TERM && iter->getType() != TOKENEND){std::cout<<"Adding:"<<iter->getValue()<<std::endl; cmd.push_back(iter->getType(),iter->getValue(),iter->scopenumber); iter++;}
        else while(iter->getType() != LOOPTERM && iter->getType() != TOKENEND){std::cout<<"Adding:"<<iter->getValue()<<std::endl;cmd.push_back(iter->getType(),iter->getValue(),iter->scopenumber); iter++;}
        cmd.push_back(TOKENEND," ",0); //REVIEW
        return_node->branches.push_back(ASTNode::assembleCmd(cmd));
        iter++;
    }
    return return_node;
}
ASTNode* ASTNode::assembleCmd(TokenSegment ts)
{
    std::cout<<"Checking command:("<<ts.getStringValue()<<")"<<std::endl;
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
    std::vector<Token>::iterator iter = ts.tokens.begin();
    std::vector<Token> line;
    line = ts.createUntil({LOOPTERM},iter,false);
    ASTNode* head = ASTNode::assembleIfHeader(line);
    line.clear();
    iter++;
    line = ts.createUntil({TOKENEND,ENDKEYWORD},iter,false);
    line.push_back(Token(TOKENEND," ",0));
    std::cout<<"NOW:"<<TokenSegment(line).getStringValue()<<std::endl;
    ASTNode* body = ASTNode::assembleCmdSeq(TokenSegment(line),true);
    return_node->branches.push_back(head);
    return_node->branches.push_back(body);
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
    unsigned int tokenlength = ts.size() - 2; //-2 for the keyword, and the TOKENEND at the end
    if(tokenlength<=0) fail("Internal Error: 'return' statement identification flawed");
    std::vector<Token>::iterator iter = ts.tokens.begin() + 1;
    TokenSegment expr;
    for(int i = 0; i < tokenlength; i ++ ) //Come to think of it, this is probably what I should have been doing the whole time
    {
    expr.tokens.push_back(*iter);
    iter++;
    }
    return_node->data["expr"] = ASTNode::assembleCmd(expr);
    return return_node;
}

bool ASTNode::checkIdentification(TokenSegment ts,Type t)
{
    switch(t){
        case IFHEADER:
        {
            ts.push_back(TOKENEND," ",0);
            if(ts.tokens[0].getType()==IFKEYWORD && ts.size() > 1) //crude definition, but should work
            {
                return true;
            }
            return false;
        }
        case RETURNCMD:
        {
            ts.push_back(TOKENEND," ",0);
            if(ts.tokens[0].getType()==RETURNKEYWORD&& ts.size() > 1)
            {
                std::cout<<"It's a return keyword!"<<std::endl;
                return true;   
            }
            if(ts.tokens[0].getType() == RETURNKEYWORD) fail("Code error: Return statement is not finished!");
            return false;
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
        result = "if("+branches[0]->getResult()+") {";
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
    default:
    {
        //fail("Unsupported token!");
        break;
    }
}
}
