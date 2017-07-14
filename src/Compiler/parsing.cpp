#include "../../Include/parsing.h"
#include "../../Include/ASTNode.h"

ASTTree* working_tree;

TokenSegment eatExpr(TokenSegment& ts)
{
    TokenSegment ret;
    for(;!ts.end();ts.next())
    {
        if ( ts.type() != NUMBER && ts.type() != OPAREN && ts.type() != CPAREN && ts.type() != MULTIPLY && ts.type() != MODULO && ts.type() != DIVIDE && ts.type() != SUBTRACT && ts.type() != ADD && ts.type() != EQUALS && ts.type() != NOTEQUALS) //This is really ugly/inefficient. We need methods to organize types
        {
            break;
        }
        ret.push_back(ts.get());
    }
    return ret;
}

TokenSegment eatForHeader(TokenSegment &ts)
{
    TokenSegment ret;
    for(;!ts.end();ts.next())
    {

        if ( ts.type() == TERM )
        {
            break;
        }
        ret.push_back(ts.get());
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
        //see if we can identify command by certain rules
        if(ts.type() == IFKEYWORD)
        {
          std::cout<<"I"<<std::endl;
            //if statement
            ts.next();
            TokenSegment expr = eatExpr(ts);
            std::cout<<"Expr:"<<expr.getStringValue();
            ts.next();
            TokenSegment body = ts.eatIndented(ts);
            std::cout<<"B:"<<body.size()<<std::endl;
            return_node->branches.push_back(assembleIf(expr,body,return_node));
            continue;
        }
	if(ts.type() == RETURNKEYWORD)
	{
    std::cout<<"R"<<std::endl;
		//return statement
		ts.next();
		TokenSegment expr = eatExpr(ts);
		ts.next();
		return_node->branches.push_back(assembleReturn(expr,return_node));
		continue;
	}
        fail("Unrecognized command!" + std::to_string((int) ts.type()) + " at " + std::to_string(ts.current()));
    }
    return return_node;
}

ASTNode* assembleVarInit(TokenSegment ts,ASTNode* parent)
{
    VarInitNode* return_node = new VarInitNode(parent);
    std::cout << "t" <<ts.size() <<std ::endl;
    //iterate through keywords
    return_node->isconst = false;
    for(ts.reset();!ts.end();ts.next())
    {
        if(ts.type() == ASYNCKEYWORD) fail("Invalid token!");
        if(ts.type() == CONSTKEYWORD) return_node->isconst = true;
    }
    return_node->vtype = ts.at(ts.size()-2).getValue();
    return_node->vname = ts.at(ts.size()-1).getValue();
    return_node->vars_defined[{return_node->vname,return_node->vtype}] = true;
    return return_node;
}
ASTNode* assembleIf(TokenSegment cmd,TokenSegment body,ASTNode* parent)
{
    ASTNode* return_node = new IfNode(parent);
    return_node->branches.push_back(assembleExpr(cmd,return_node));
    return_node->branches.push_back(assembleCmdSeq(body,return_node));
    return return_node;
}

ASTNode* assembleReturn(TokenSegment expr,ASTNode* parent)
{
	ReturnNode* return_node = new ReturnNode(parent);
	return_node->ret_expr = assembleExpr(expr,return_node);
	return return_node;
}
namespace local
{
  void stripExtraneousParen(TokenSegment& ts)
  {
    unsigned int paren_in = 0;
    /*
    while(ts.end())
    {

    }
    */
    //ts.tokens.erase(ts.tokens.end());
    //ts.tokens.erase(ts.tokens.begin());
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
  std::string getOperatorAsString(int op)
  {
    switch(op)
    {
      case ADD:
        return "+";
      case SUBTRACT:
        return "-";
      case DIVIDE:
        return "/";
      case MODULO:
        return "%";
      case EQUALS:
        return "==";
      case NOTEQUALS:
        return "!=";
    }
    fail("Operator unrecognized!");
  }
}
ASTNode* assembleExpr(TokenSegment expr,ASTNode* parent)
{
    ExprNode* return_node = new ExprNode(parent);
    //strip parentheses
    local::stripExtraneousParen(expr);
    //determine type of expr by iterating over TokenSegment
    unsigned int type = 0,op_iter=0;
    while(!expr.end())
    {
      //if(expr.type() == OPAREN) paren_in++;
      //if(expr.type() == CPAREN) paren_in--;
      std::cout<<expr.get().getValue()<<":"<<local::getOperatorPriority(expr.type())<<std::endl;
      if(local::getOperatorPriority(expr.type()) < local::getOperatorPriority(expr.at(op_iter).getType()))
      {
        op_iter = expr.current();
        return_node->operation = expr.type();
        std::cout<<"Naming:"<<op_iter<<" "<<return_node->operation<<std::endl;
      }
      expr.next();
    }
      if(op_iter==0)
      {
        std::cout<<"Ret"<<std::endl;
        return_node->endpoint_val = expr.getStringValue();
        return_node->branches.push_back(assembleEndpoint(expr,return_node));
        return return_node;
      }
      return_node->type = 2;
        TokenSegment left,right;
        expr.reset();
        while(expr.current() != op_iter)
        {
          left.tokens.push_back(expr.get());
          expr.next();
        }
        expr.next(); //skips operator
        while(!expr.end())
        {
          right.tokens.push_back(expr.get());
          expr.next();
        }
        return_node->branches.push_back(assembleExpr(left,return_node));
        return_node->branches.push_back(assembleExpr(right,return_node));
        return return_node;
}

ASTNode* assembleEndpoint(TokenSegment ts,ASTNode* parent)
{
    EndpointNode* return_node = new EndpointNode(parent);
    return_node->string_comp = ts.getStringValue();
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
    ts.next();
    return_node->vars_defined = return_node->branches[0]->vars_defined;
    working_tree->mapVar(return_node->vars_defined.begin()->first.first,return_node->vars_defined.begin()->first.second,return_node);
    std::cout<<"?:"<<working_tree->check("thing",return_node)<<std::endl;
    TokenSegment commandseq = ts.eatIndented(ts);
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

ASTNode* assembleFunc(TokenSegment ts,ASTNode* parent)
{
    FuncNode* return_node = new FuncNode(parent);
    return return_node;
}

ASTNode* assembleParenList(TokenSegment ts,ASTNode* parent)
{
    ParenList* return_node = new ParenList(parent);
    return return_node;
}


void ExprNode::assemble(){
  std::cout<<"WHAT";
    for(auto b : branches){ b->assemble(); std::cout<<b->finished_result<<","<<std::endl; }
    std::cout<<operation<<std::endl;
    if(type == 0){ finished_result = "(" + branches[0]->finished_result + ")"; return;}
    finished_result += "(" + branches[0]->finished_result + local::getOperatorAsString(operation) + branches[1]->finished_result + ")";
}
void ASTNode::assemble(){
    for(auto b : branches)
        {
            b->assemble();
            finished_result += b->finished_result;
        }
}
void VarInitNode::assemble(){
    if(isconst) finished_result += "const "; //Make sure spaces are included after we append a keyword
    finished_result += vtype + " " + vname;
}
void IfNode::assemble(){
    for(auto b : branches) b->assemble();
    finished_result = "if " + branches[0]->finished_result + "{\n" + branches[1]->finished_result + "}";
}
void CmdSeqNode::assemble(){
    for(auto b : branches) b->assemble();
    for(auto b : branches)
    {
        finished_result += b->finished_result + ";\n";
    }
}

void EndpointNode::assemble() {
    finished_result = string_comp;
}
void ForNode::assemble() {
    std::cout<<branches[1]->branches.size()<<std::endl;
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

void ReturnNode::assemble()
{
	ret_expr->assemble();
	finished_result = "return " + ret_expr->finished_result;
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

void ParenList::assemble()
{
    for(auto b : branches){ b->assemble(); finished_result += b->finished_result;}
}

void FuncNode::assemble()
{

}
