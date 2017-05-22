#include "../../Include/parsing.h"
#include "../../Include/ASTNode.h"

TokenSegment eatExpr(unsigned int& i,TokenSegment ts) //Incoporate into object, maybe?
{
    TokenSegment ret;
    for(;i<ts.size();i++)
    {
        ret.tokens.push_back(ts.tokens[i]);
        if ( ts.tokens[i].getType() != NUMBER && ts.tokens[i].getType() != OPAREN && ts.tokens[i].getType() != CPAREN && ts.tokens[i].getType() != MULTIPLY && ts.tokens[i].getType() != MODULO && ts.tokens[i].getType() != DIVIDE && ts.tokens[i].getType() != SUBTRACT && ts.tokens[i].getType() != ADD && ts.tokens[i].getType() != EQUALS && ts.tokens[i].getType() != NOTEQUALS) //This is really ugly/inefficient. We need methods to organize types
        {
            break;
        }
    }
    return ret;
}

TokenSegment eatVarKeywords(unsigned int& i, TokenSegment ts)
{
    TokenSegment ret;
    for(;i<ts.size();i++)
    {
        ret.tokens.push_back(ts.tokens[i]);
        if( ts.tokens[i].getType() != ASYNCKEYWORD && ts.tokens[i].getType() != CONSTKEYWORD && ts.tokens[i].getType() != PTRKEYWORD && ts.tokens[i].getType() != SAFEKEYWORD && ts.tokens[i].getType() != REFKEYWORD)
        {
            break;
        }
    }
    return ret;
}

ASTNode* assembleTop(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode();
    return_node = assembleCmdSeq(ts);
    return return_node;
}


ASTNode* assembleCmdSeq(TokenSegment ts)
{
    std::cout<<"Checking command sequence "<<ts.getStringValue()<<std::endl;
    ASTNode* return_node = new CmdSeqNode();
    unsigned int i = 0;
    while(i < ts.tokens.size())
    {
        TokenSegment cmd;
        cmd = ts.createUntil({TERM},i,ts,false);
        //if unique command detected, continue
        if(cmd.size()==0) break; //fixes bug
        TokenSegment temp;
        if(IfNode::is(cmd))
        {
            std::cout<<"Identified as if statement."<<std::endl;
            temp = ts.createUntil({ENDKEYWORD},i,ts,true);
            i++;
            for(auto t : temp.tokens)
            {
                cmd.tokens.push_back(t);
            }
            return_node->branches.push_back(assembleIf(cmd));
            i++;
            continue;
        }
        else if(ForNode::is(cmd))
        {
            std::cout<<"Identified as for statement."<<std::endl;
            temp = ts.createUntil({ENDKEYWORD},i,ts,true);
            i++;
            for(auto t : temp.tokens)
            {
                cmd.tokens.push_back(t);
            }
            return_node->branches.push_back(assembleIf(cmd)); //FIX
            i++;
            continue;
        }
        else if(VarNode::is(cmd));
        {
            std::cout<<"Identified as variable"<<std::endl;
            return_node->branches.push_back(assembleVarInit(cmd));
            i++;
            continue;
        }
        fail("Unrecognized command!");
    }
    return return_node;
}

ASTNode* assembleVarInit(TokenSegment ts)
{
    ASTNode* return_node = new VarNode();
    //iterate through keywords
    return_node->node_data.data["const"] = "F";
    for(int i = 0; i < ts.size()-2; i++)
    {
        if(ts.tokens[i].getType() == ASYNCKEYWORD) fail("Invalid token!");
        if(ts.tokens[i].getType() == CONSTKEYWORD) return_node->node_data.data["const"] = "T";
    }
    return_node->node_data.data["type"] = ts.tokens[ts.size()-2].getValue();
    return_node->node_data.data["name"] = ts.tokens[ts.size()-1].getValue();
    return return_node;
}
ASTNode* assembleIf(TokenSegment ts)
{
    ASTNode* return_node = new IfNode();
    unsigned int i = 1; //skips if keyword
    return_node->branches.push_back(assembleExpr(eatExpr(i,ts))); //expression top
    i++; //eliminates newline
    std::cout<<"I:"<<i<<std::endl;
    TokenSegment commandseq = ts.createUntil({ENDKEYWORD},i,ts,true);
    return_node->branches.push_back(assembleCmdSeq(commandseq));
    return return_node;
}
/*
ASTNode* assembleIf(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(IF);
    unsigned int i = 0;
    TokenSegment line;
    line = ts.createUntil({TERM},i,ts,false);
    ASTNode* head = assembleIfHeader(line);
    line.tokens.clear();
    i++;
    line = ts.createUntil({ENDKEYWORD},i,ts,true);
    //line.push_back(Token(ENDKEYWORD,"end",line[line.size()-1].scopenumber));
    std::cout<<"Assembling line:"<<TokenSegment(line).getStringValue()<<std::endl;
    ASTNode* body = assembleCmdSeq(TokenSegment(line),true);
    return_node->branches.push_back(head);
    return_node->branches.push_back(body);
    std::cout<<"Closing if"<<std::endl;
    return return_node; //NEXT add header cmds and generalize looping. FUTURE: add functions
}
*/
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

ASTNode* assembleExpr(TokenSegment ts)
{
    std::cout<<"Starting parsing"<<std::endl;
    ASTNode* return_node = new ExprNode();
    //TODO: check if the expression is binary, singular, or nonoperational (ie. just a variable or constant)
    unsigned int a = 0;
    TokenSegment left_expr;
    unsigned int paren_in = 0,i=0;
    bool paren_wrap = false;
    while(i < ts.size())
    {
        if(((ts.tokens[i].getType() == ADD && ts.tokens[i].getType() == SUBTRACT && ts.tokens[i].getType() == DIVIDE) && paren_in == 0))
        {
            paren_wrap = true;
        }
        if(ts.tokens[i].getType() == OPAREN){ paren_in++; std::cout<<"+"<<std::endl; }
        if(ts.tokens[i].getType() == CPAREN){ paren_in--; std::cout<<"-"<<std::endl; }
        i++;
    }
    if(paren_wrap) //then we have to unwrap some parentheses
    {
        while(left_expr.tokens[0].getType() == OPAREN && left_expr.tokens[left_expr.tokens.size()-1].getType() == CPAREN)
        {
            left_expr.tokens.erase(ts.tokens.begin());
            left_expr.tokens.erase(ts.tokens.end()-1);
            std::cout<<"erasing"<<std::endl;
        }
    }
    //then calculate
    i = 0;
    unsigned int least_prominent_operator_iterator = 999;
    while(i < ts.size())
    {
        //get the prominent operator
        std::cout<<"operator priority:"<<getOperatorPriority(ts.tokens[i].getType());
        if((getOperatorPriority(ts.tokens[i].getType()) < getOperatorPriority(ts.tokens[least_prominent_operator_iterator].getType())) && paren_in == 0)
        {
            std::cout<<"Naming least prominent operator!"<<std::endl;
            least_prominent_operator_iterator = i;
        }
        if(ts.tokens[i].getType() == OPAREN) paren_in++;
        if(ts.tokens[i].getType() == CPAREN) paren_in--;
        i++;
    }
    if(least_prominent_operator_iterator == 999) //weak
    {
        return_node->node_data.data["end"] = "T";
        return_node->branches.push_back(assembleEndpoint(ts));
        std::cout<<"breaking"<<std::endl;
        return return_node;
    }
    return_node->node_data.data["end"] = "F";
    i = 0;
    while(i < least_prominent_operator_iterator)
    {
        left_expr.tokens.push_back(ts.tokens[i]);
        i++;
    }
    std::cout<<ts.tokens[i].getType()<<std::endl;
    return_node->node_data.data["op"] = ts.tokens[i].getValue();
    i++;
    TokenSegment right_expr;
    while(i < ts.tokens.size())
        {
            right_expr.tokens.push_back(ts.tokens[i]);
            i++;
        }
    return_node->branches.push_back(assembleExpr(left_expr));
    return_node->branches.push_back(assembleExpr(right_expr));

    return return_node;
}
ASTNode* assembleEndpoint(TokenSegment ts)
{
    ASTNode* return_node = new EndpointNode();
    return_node->node_data.data["value"] = ts.getStringValue();
    return return_node;
}
/*
ASTNode* assembleIfHeader(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(IFHEADER);
    std::cout<<"Assembling header!";
    unsigned int i = 1;
    TokenSegment header = ts.createUntil({TERM},i,ts,false);
    return_node->data["expr"] = assembleExpr(header); //CHANGE
    return return_node;
}

ASTNode* assembleReturnCmd(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(RETURNCMD);
    unsigned int tokenlength = ts.size() - 1; //1 for the return keyword
    if(tokenlength<=0) fail("Internal Error: 'return' statement identification flawed");
    unsigned int i = 1;
    TokenSegment expr;
    expr = ts.createUntil({TERM},i,ts,false);
    return_node->data["expr"] = assembleCmd(expr);
    return return_node;
}

ASTNode* assembleFor(TokenSegment ts)
{
    //this should have an ending statement. If a ending statement can not be found, we have a parsing problem
    ASTNode* return_node = new ASTNode(FOR);
    unsigned int i = 0;
    TokenSegment header = ts.createUntil({TERM},i,ts,false);
    ASTNode* head = assembleForHeader(header);
    i++;
    TokenSegment interior = ts.createUntil({ENDKEYWORD},i,ts,true);
    ASTNode* body = assembleCmdSeq(interior,false);
    return_node->branches.push_back(head);
    return_node->branches.push_back(body);
    std::cout<<"Closing for"<<std::endl;
    return return_node;
}

ASTNode* assembleForHeader(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(FORHEADER);
    unsigned int i = 4;
    if(ts.tokens[i].getType()==AT)
    {
        //range
        i-=3;
        TokenSegment variable;
        for(int a = 0; a < 3; a++)
        {
            variable.tokens.push_back(ts.tokens[a+i]);
        }
        return_node->data["var"] = assembleVarInit(variable);
        i += 4;
        return_node->data["range"] = assembleCmd(ts.tokens[i]);
    }
    else {
        fail("Parsing error: 'For' statement contained a unexpected token");
    }
    return return_node;
}
bool checkIdentification(TokenSegment ts,Type t)
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
            if(ts.tokens[0].getType()==FORKEYWORD && ts.size() > 3)
            {
                std::cout<<"It's a 'for' header!"<<std::endl;
                return true;
            }
            if(ts.tokens[0].getType() == FORKEYWORD && ts.size() <= 3) fail("Code error: For statement is not finished!");
            return false;
            break;
        }
        default:
        {
            return false;
        }
    }
}
/*
ASTNode* assembleLoop(TokenSegment ts)
{

}
*/
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
void VarNode::assemble(){
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
/*
void ASTNode::assemble(){
    fail("Abstract class is being assembled!");
}
void ASTNode::assemble(){
    fail("Abstract class is being assembled!");
}
void ASTNode::assemble(){
    fail("Abstract class is being assembled!");
}
*/
/*
void assemble()
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
    case EXPR:
    {
        if(meta == "ADD" || meta == "SUBTRACT" || meta == "DIVIDE" || meta == "MODULO" || meta == "EQUALS" || meta == "NOTEQUALS")
        {
        data["left"]->assemble(); data["right"]->assemble();
        result += data["left"]->getResult();
        result += meta=="ADD"?"+" :  meta=="SUBTRACT" ?"-" : meta=="MODULO" ? "%" : meta=="EQUALS"? "==" : meta=="NOTEQUALS" ? "!=" : "/";
        result += data["right"]->getResult();
        }
        else{
            result = token;
        }
        break;
    }
    case IFHEADER:
    {
        data["expr"]->assemble();
        result = data["expr"]->getResult();
        break;
    }
    case RETURNCMD:
    {
        result = "return " + data["expr"]->getToken();
        break;
    }
    case FORHEADER:
    {
        data["var"]->assemble();
        data["range"]->assemble();
        result = "for(" + data["var"]->getResult() + "= 0;" + data["var"]->data["name"]->getToken() + " < " + data["range"]->getResult() + ";" + data["var"]->data["name"]->getToken() + "++)";
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
    case VARINIT:
    {
        result = data["type"]->getToken() + " " + data["name"]->getToken();
        break;
    }
    default:
    {
        //fail("Unsupported token!");
        break;
    }
}
}
*/