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
            std::cout<<"Identified as if statement."<<std::endl;
            temp = ts.createUntil({ENDKEYWORD},i,ts,true);
            i++;
        }
        else if(checkIdentification(cmd,FORHEADER))
        {
            std::cout<<"Identified as for statement."<<std::endl;
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
    else if(checkIdentification(ts,FORHEADER))
    {
        return_node = ASTNode::assembleFor(ts);
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
ASTNode* ASTNode::assembleVarInit(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(VARINIT);
    return_node->data["name"] = new ASTNode(CMD);
    return_node->data["name"]->token = ts.tokens[0].getValue();
    std::cout<<"CHECKING:"<<return_node->data["name"]->token<<std::endl;
    return_node->data["type"] = new ASTNode(CMD);
    return_node->data["type"]->token = ts.tokens[2].getValue();
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
int getOperatorPriority(TokenType operatortt) //local function
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
    }
    return 999;
}
ASTNode* ASTNode::assembleExpr(TokenSegment ts)
{
    std::cout<<"Starting parsing"<<std::endl;
    ASTNode* return_node = new ASTNode(EXPR);
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
    unsigned int least_prominent_operator_iterator = 0;
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
    if(least_prominent_operator_iterator == 0) //weak 
    {
        return_node->token = ts.getStringValue();
        std::cout<<"breaking"<<std::endl;
        return return_node;
    }
    i = 0;
    while(i < least_prominent_operator_iterator) 
    {
        left_expr.tokens.push_back(ts.tokens[i]);
        i++;
    }
    //TODO: Maybe move logical component to another location?
    if(ts.tokens[i].getType() == ADD) return_node->meta = "ADD";
    else if(ts.tokens[i].getType() == SUBTRACT) return_node->meta = "SUBTRACT";
    else if(ts.tokens[i].getType() == DIVIDE) return_node->meta = "DIVIDE";
    else if(ts.tokens[i].getType() == MODULO) return_node->meta = "MODULO";
    i++;
    std::cout<<"Left Expression Parsed. Result:"<<left_expr.getStringValue()<<std::endl;

    TokenSegment right_expr;
    while(i < ts.tokens.size())
        {
            right_expr.tokens.push_back(ts.tokens[i]);
            i++;
        }
            std::cout<<"Right Expression Parsed. Result:"<<right_expr.getStringValue()<<std::endl;
    return_node->data["left"] = ASTNode::assembleExpr(left_expr);
    return_node->data["right"] = ASTNode::assembleExpr(right_expr);

    return return_node;
}
ASTNode* ASTNode::assembleIfHeader(TokenSegment ts)
{
    ASTNode* return_node = new ASTNode(IFHEADER);
    std::cout<<"Assembling header!";
    unsigned int i = 1;
    TokenSegment header = ts.createUntil({TERM},i,ts,false);
    return_node->data["expr"] = ASTNode::assembleExpr(header); //CHANGE
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
        return_node->data["var"] = ASTNode::assembleVarInit(variable);
        i += 4;
        return_node->data["range"] = ASTNode::assembleCmd(ts.tokens[i]);
    }
    else {
        fail("Parsing error: 'For' statement contained a unexpected token");
    }
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
    case EXPR:
    {
        if(meta=="ADD" || meta=="SUBTRACT" || meta=="DIVIDE" || meta=="MODULO")
        {
        result = "(";
        data["left"]->assemble(); data["right"]->assemble();
        result += data["left"]->getResult();
        std::cout<<"META:"<<meta<<std::endl;
        result += meta=="ADD"?"+" :  meta=="SUBTRACT" ?"-" : meta=="MODULO" ? "%" : "/";
        result += data["right"]->getResult();
        result += ")";
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
