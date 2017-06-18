#include "../../Include/parsing.h"
#include "../../Include/ASTNode.h"

ASTTree* working_tree;

TokenSegment eatExpr(TokenSegment& ts)
{
    TokenSegment ret;
    for(;!ts.end();ts.next())
    {
        ret.push_back(ts.get());
        if ( ts.type() != NUMBER && ts.type() != OPAREN && ts.type() != CPAREN && ts.type() != MULTIPLY && ts.type() != MODULO && ts.type() != DIVIDE && ts.type() != SUBTRACT && ts.type() != ADD && ts.type() != EQUALS && ts.type() != NOTEQUALS) //This is really ugly/inefficient. We need methods to organize types
        {
            break;
        }
    }
    return ret;
}

TokenSegment eatForHeader(TokenSegment &ts)
{
    TokenSegment ret;
    for(;!ts.end();ts.next())
    {
        ret.push_back(ts.get());
        if ( ts.type() == TERM )
        {
            break;
        }
    }
    return ret;
}

TokenSegment eatVarKeywords(TokenSegment& ts)
{
    TokenSegment ret;
    for(;!ts.end();ts.next())
    {
        if( ts.type() != ASYNCKEYWORD && ts.type() != CONSTKEYWORD && ts.type() != PTRKEYWORD && ts.type() != SAFEKEYWORD && ts.type() != REFKEYWORD)
        {
            break;
        }
        ret.push_back(ts.get());
    }
    return ret;
}

ASTNode* assembleTop(TokenSegment ts,ASTTree* tree)
{
    working_tree = tree;
    ASTNode* return_node = new ASTNode(NULL);
    return_node = assembleCmdSeq(ts,return_node);
    return return_node;
}


ASTNode* assembleCmdSeq(TokenSegment ts,ASTNode* parent)
{
    std::cout<<"Checking command sequence "<<ts.getStringValue()<<std::endl;
    ASTNode* return_node = new CmdSeqNode(parent);
    while(!ts.end())
    {
        TokenSegment cmd;
        cmd = ts.createUntil({TERM},ts,false);
        //if unique command detected, continue
        if(cmd.size()==0) break; //fixes bug
        TokenSegment temp;
        if(IfNode::is(cmd))
        {
            std::cout<<"Identified as if statement."<<std::endl;
            temp = ts.createUntil({ENDKEYWORD},ts,true);
            ts.next();
            for(temp.reset();!temp.end();temp.next())
            {
                cmd.push_back(temp.get());
            }
            return_node->branches.push_back(assembleIf(cmd,return_node));
            ts.next();
            continue;
        }
        else if(ForNode::is(cmd))
        {
            std::cout<<"Identified as for statement."<<std::endl;
            temp = ts.createUntil({ENDKEYWORD},ts,true);
            ts.next();
            for(temp.reset();!temp.end();temp.next())
            {
                cmd.push_back(temp.get());
            }
            return_node->branches.push_back(assembleFor(cmd,return_node));
            ts.next();
            continue;
        }
        else if(VarInitNode::is(cmd))
        {
            std::cout<<"Identified as variable"<<std::endl;
            return_node->branches.push_back(assembleVarInit(cmd,return_node));
            ts.next();
            continue;
        }
        fail("Unrecognized command!");
    }
    return return_node;
}

ASTNode* assembleVarInit(TokenSegment ts,ASTNode* parent)
{
    ASTNode* return_node = new VarInitNode(parent);
    //iterate through keywords
    return_node->node_data.data["const"] = "F";
    for(ts.reset();!ts.end();ts.next())
    {
        if(ts.type() == ASYNCKEYWORD) fail("Invalid token!");
        if(ts.type() == CONSTKEYWORD) return_node->node_data.data["const"] = "T";
    }
    return_node->node_data.data["type"] = ts.at(ts.size()-2).getValue();
    return_node->node_data.data["name"] = ts.at(ts.size()-1).getValue();
    return_node->vars_defined[{return_node->node_data.data["name"],return_node->node_data.data["type"]}] = true;
    return return_node;
}
ASTNode* assembleIf(TokenSegment ts,ASTNode* parent)
{
    std::cout<<"Called at"<<ts.current()<<std::endl;
    ASTNode* return_node = new IfNode(parent);
    ts.next();
    return_node->branches.push_back(assembleExpr(eatExpr(ts),parent)); //expression top
    ts.next(); //eliminates newline
    TokenSegment commandseq = ts.createUntil({ENDKEYWORD},ts,true);
    return_node->branches.push_back(assembleCmdSeq(commandseq,return_node));
    return return_node;
}

int getOperatorPriority(TokenType operatortt)
{
    switch(operatortt)
    {
        case ADD:
        case SUBTRACT:
            return 1;
        case DIVIDE:
            return 2;
        case MODULO:
            return 3;
        case EQUALS:
        case NOTEQUALS:
            return 20;
    }
    return 999;
}

ASTNode* assembleExpr(TokenSegment ts,ASTNode* parent)
{
    std::cout<<"Starting parsing"<<std::endl;
    ASTNode* return_node = new ExprNode(parent);
    //TODO: check if the expression is binary, singular, or nonoperational (ie. just a variable or constant)
    //unsigned int a = 0;
    TokenSegment left_expr,right_expr;
    unsigned int paren_in = 0;
    bool paren_wrap = false;
    while(!ts.end())
    {
        if(((ts.type() == ADD && ts.type() == SUBTRACT && ts.type() == DIVIDE) && paren_in == 0))
        {
            paren_wrap = true;
        }
        if(ts.type() == OPAREN){ paren_in++; std::cout<<"+"<<std::endl; }
        if(ts.type() == CPAREN){ paren_in--; std::cout<<"-"<<std::endl; }
        ts.next();
    }
    if(paren_wrap) //then we have to unwrap some parentheses
    {
        while(left_expr.at(0).getType() == OPAREN && left_expr.at(left_expr.size()-1).getType() == CPAREN)
        {
            left_expr.erase(ts.front());
            left_expr.erase(ts.back()-1);
            std::cout<<"erasing"<<std::endl;
        }
    }
    //then calculate
    ts.reset();
    unsigned int least_prominent_operator_iterator = 0;
    while(!ts.end())
    {
        //get the prominent operator
        std::cout<<"operator priority:"<<getOperatorPriority(ts.type())<<std::endl;
        if((getOperatorPriority(ts.type()) < getOperatorPriority(ts.at(least_prominent_operator_iterator).getType())) && paren_in == 0)
        {
            std::cout<<"Naming least prominent operator!"<<std::endl;
            least_prominent_operator_iterator = ts.current();
        }
        if(ts.type() == OPAREN) paren_in++;
        if(ts.type() == CPAREN) paren_in--;
        ts.next();
    }
    if(least_prominent_operator_iterator == 0) //weak, there must be a better way to do this...
    {
        return_node->node_data.data["end"] = "T";
        return_node->branches.push_back(assembleEndpoint(ts,parent));
        std::cout<<"breaking"<<std::endl;
        return return_node;
    }
    return_node->node_data.data["end"] = "F";
    ts.reset();
    while(ts.current() < least_prominent_operator_iterator)
    {
        left_expr.push_back(ts.get());
        ts.next();
    }
    std::cout<<ts.type()<<std::endl;
    return_node->node_data.data["op"] = ts.value();
    ts.next();
    while(!ts.end())
        {
            right_expr.push_back(ts.get());
            ts.next();
        }
    return_node->branches.push_back(assembleExpr(left_expr,return_node));
    return_node->branches.push_back(assembleExpr(right_expr,return_node));
    return return_node;
}

ASTNode* assembleEndpoint(TokenSegment ts,ASTNode* parent)
{
    ASTNode* return_node = new EndpointNode(parent);
    return_node->node_data.data["value"] = ts.getStringValue();
    return return_node;
}

ASTNode* assembleForHeader(TokenSegment ts,ASTNode* parent)
{
    ASTNode* return_node = new ForHeaderNode(parent);
    TokenSegment var = eatVarKeywords(ts);
    std::cout<<var.getStringValue()<<std::endl;
    var.push_back(ts.get());
    ts.next();
    var.push_back(ts.get());
    return_node->branches.push_back(assembleVarInit(var,return_node));
    return_node->vars_defined = return_node->branches[0]->vars_defined;
    //our current iterator should be the variable name
    ts.next();ts.next();
    TokenSegment secondpart;
    while(!ts.end())
    {
        secondpart.push_back(ts.get());
        ts.next();
    }
    return_node->branches.push_back(assembleEndpoint(secondpart,return_node));
    return return_node;
}

ASTNode* assembleFor(TokenSegment ts,ASTNode* parent)
{
    ASTNode* return_node = new ForNode(parent);
    ts.next();
    TokenSegment head = eatForHeader(ts);
    std::cout<<head.getStringValue()<<std::endl;
    //do stuff with the header
    return_node->branches.push_back(assembleForHeader(head,return_node));
    return_node->vars_defined = return_node->branches[0]->vars_defined;
    working_tree->mapVar(return_node->vars_defined.begin()->first.first,return_node->vars_defined.begin()->first.second,return_node);
    std::cout<<"?:"<<working_tree->check(return_node->vars_defined.begin()->first.first,return_node)<<std::endl;
    TokenSegment commandseq = ts.createUntil({ENDKEYWORD},ts,true);
    return_node->branches.push_back(assembleCmdSeq(commandseq,return_node));
    return return_node;
}

ASTNode* assembleVarNode(TokenSegment ts,ASTNode* parent)
{
    ASTNode* return_node = new VarNode(parent);
    if(!working_tree->check(ts.getStringValue(),return_node))
    {
        std::cout<<"???"<<std::endl;
    }
    return return_node;
}

void ExprNode::assemble(){
    for(auto b : branches) b->assemble();
    if(node_data.data["end"]=="T"){ finished_result = branches[0]->finished_result; return;}
    finished_result += branches[0]->finished_result + node_data.data["op"] + branches[1]->finished_result;
}
void ASTNode::assemble(){
    for(auto b : branches)
        {
            b->assemble();
            finished_result += b->finished_result;
        }
}
void VarInitNode::assemble(){
    if(node_data.data["const"] == "T") finished_result += "const "; //Make sure spaces are included after we append a keyword
    finished_result += node_data.data["type"] + " " + node_data.data["name"];
}
void IfNode::assemble(){
    for(auto b : branches) b->assemble();
    finished_result = "if (" + branches[0]->finished_result + "){\n" + branches[1]->finished_result + "}";
}
void CmdSeqNode::assemble(){
    for(auto b : branches) b->assemble();
    for(auto b : branches)
    {
        finished_result += b->finished_result + ";\n";
    }
}

void EndpointNode::assemble() {
    finished_result = node_data.data["value"];
}

void ForNode::assemble() {
    for(auto b : branches)
    {
        b->assemble();
    }
    finished_result = "for (" + branches[0]->finished_result + ")";
    finished_result += "{\n" + branches[1]->finished_result + "}";
}

void ForHeaderNode::assemble() {
    for(auto b : branches){ b->assemble();}
    finished_result = branches[0]->finished_result + " : " + branches[1]->finished_result;
}

void VarAssignNode::assemble()
{

}
void VarInitANode::assemble()
{

}
void VarNode::assemble()
{

}
