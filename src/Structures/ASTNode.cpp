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
        line_delimiter_iter++;
        line.push_back(*line_delimiter_iter); 
        
        if(ASTNode::checkIdentification(line,IFHEADER))
        { //LATER: Generalize loop
            while(line_delimiter_iter->getType()!=ENDKEYWORD)
            {
                line_delimiter_iter++;
                line.push_back(*line_delimiter_iter);
            }
            line.push_back(Token(TOKENEND," ",0)); //We always tack on a TOKENEND when creating a new TokenSegment
            return_node->branches.push_back(assembleIf(TokenSegment(line)));
            loop = true;
            line.clear();
            break; //REDO so that we can have standardly defined parsing
        }
    }
    std::cout<<loop<<std::endl;
    if(!loop){
    return_node->branches.push_back(assembleCmd(TokenSegment(line))); 
    line.clear();
    } else loop = false;
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
    return_node->branches.push_back(assembleCmdSeq(body));
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

ASTNode* ASTNode::assembleCmdSeq(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(CMDSEQ);
    std::vector<Token>::iterator iter;
    while(iter->getType() != TOKENEND)
    {
        TokenSegment cmd;
        while(iter->getType() != TERM){ iter++; cmd.push_back(iter->getType(),iter->getValue(),iter->scopenumber); }
        cmd.push_back(TOKENEND," ",0); //REVIEW
        return_node->branches.push_back(ASTNode::assembleCmd(cmd));
    }
    return return_node;
}
ASTNode* ASTNode::assembleCmd(TokenSegment ts)
{
    std::cout<<"Checking command:("<<ts.getStringValue()<<")"<<std::endl;
    ASTNode* return_node = new ASTNode(CMD);
    if(ts.tokenSequencePresent({ENDKEYWORD})) fail("Internal error: End keyword being processed as a command!");
    //else if(ASTNode::checkIdentification(ts,VARDEC)) return_node->branches.push_back(ASTNode::assembleVariableDeclaration(ts));
    //else if(ASTNode::checkIdentification(ts,VARSET)) return_node->branches.push_back(ASTNode::assembleVariableAssignment(ts));
    //else if(ASTNode::checkIdentification(ts,VARDEC)) return_node->branches.push_back(ASTNode::assembleVariableDeclaration(ts));
    return return_node;
}
ASTNode* ASTNode::assembleIf(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(IF);
    std::vector<Token>::iterator iter = ts.tokens.begin();
    std::vector<Token> line;
    line = ts.createUntil(TERM,iter);
    ASTNode* head = ASTNode::assembleIfHeader(line);
    line.clear();
    //review this part, and see how I can integrate it with the reusable function
    while(iter->getType()!=TOKENEND)
    {
        iter++;
        if(iter->getType()==TOKENEND || iter->getType() == ENDKEYWORD) continue;
        line.push_back(*iter);
    }
    line.push_back(Token(TOKENEND," ",0));
    std::cout<<TokenSegment(line).getStringValue()<<std::endl;
    line = ts.createUntil(TOKENEND,iter);
    ASTNode* body = ASTNode::assembleCmdSeq(line);
    return_node->branches.push_back(head);
    return_node->branches.push_back(body);
    return return_node; //NEXT add header cmds and generalize looping. FUTURE: add functions
}

ASTNode* ASTNode::assembleIfHeader(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(IFHEADER);
    std::cout<<"Assembling header!";
    //return_node->data["expr"] = ASTNode::assembleCmd(ts.getBetween(TEXT,TERM));
    return return_node;
}

bool ASTNode::checkIdentification(TokenSegment ts,Type t)
{
    switch(t){
        case IFHEADER:
        {
            if(ts.tokenSequencePresent({IFKEYWORD,/*BOOLEXPR*/TEXT})) 
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
    return false;
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
            this->result += b->getResult() + ',';
        }
        break;
    }

    case IF:
    {
        branches[0]->assemble();
        result = "if("+branches[0]->getResult()+") {";
        break; //This is only partial
    }
}
}
