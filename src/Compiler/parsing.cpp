#include "../../Include/parsing.h"
#include "../../Include/ASTNode.h"

ASTTree* working_tree;

TokenSegment eatUntil(std::initializer_list<TokenType> tlist,TokenSegment& ts)
{
  TokenSegment ret;
  bool processing = true;
  while(!ts.end() && processing)
  {
    for(auto il : tlist)
    {
      if(ts.type() == il){
        processing = false;
        break;
      }
    }
    if(processing)
    {
      ret.push_back(ts.get());
      ts.next();
    }
  }
  return ret;
}

TokenSegment eatExpr(TokenSegment& ts)
{
    TokenSegment ret;
    for(;!ts.end();ts.next())
    {
        if ( ts.type() != NUMBER && ts.type() != OPAREN && ts.type() != CPAREN && ts.type() != MULTIPLY && ts.type() != MODULO && ts.type() != DIVIDE && ts.type() != SUBTRACT && ts.type() != ADD && ts.type() != EQUALS && ts.type() != NOTEQUALS && ts.type() != TEXT) //This is really ugly/inefficient. We need methods to organize types
        {
            break;
        }
        ret.push_back(ts.get());
    }
    return ret;
}

TokenSegment eatVar(TokenSegment& ts)
{
  TokenSegment ret;
  while(!ts.end())
  {
  if(ts.type() != TEXT && ts.type() != NUMBER && ts.type() != ASSIGNMENT && ts.type() != OPAREN && ts.type() != CPAREN && ts.type() != MULTIPLY && ts.type() != MODULO && ts.type() != DIVIDE && ts.type() != SUBTRACT && ts.type() != ADD && ts.type() != EQUALS && ts.type() != NOTEQUALS && ts.type() != TEXT)
  {
    break;
  }
  ret.push_back(ts.get());
  ts.next();
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
namespace local
{
  bool locateElse(TokenSegment& tsi)
  {
    TokenSegment ts = tsi;
    ts.next();
    int scope = ts.scope();
    while((ts.type()!=IFKEYWORD || ts.scope() != scope) && !ts.end() && ts.scope() >= scope){
      if(ts.type()==ELSEKEYWORD && ts.scope() == scope) return true;
      ts.next();
    }
    return false;
  }
  enum looptypes
  {
    IF,
    FOR,
    WHILE,
    SWITCH,
    FOREACH,
    IFELSE,
    CASE
  };
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
ASTNode* assembleTop(TokenSegment ts,ASTTree* tree)
{
    working_tree = tree;
    ASTNode* return_node = new ASTNode(NULL);
    return_node = assembleCmdSeq(ts,return_node);
    return return_node;
}


ASTNode* assembleCmdSeq(TokenSegment ts,ASTNode* parent)
{
    debug("Checking command sequence "+ ts.getStringValue());
    ASTNode* return_node = new CmdSeqNode(parent);
    while(!ts.end())
      {
        //see if we can identify command by certain rules
        if(ts.type() == IFKEYWORD)
        {
            //if or if/else
            if(local::locateElse(ts))
            {
              debug("IE");
              //if/else
              TokenSegment expr = eatForHeader(ts);
              ts.next();
              TokenSegment body = ts.eatIndented(ts);
              TokenSegment expr2 = eatForHeader(ts); //absorb else statement
              ts.next();
              TokenSegment body2 = ts.eatIndented(ts);
              body.tokens.reserve(body.tokens.size() + body2.tokens.size() + expr2.tokens.size());
              body.tokens.insert(body.tokens.end(),expr2.tokens.begin(),expr2.tokens.end());
              body.tokens.insert(body.tokens.end(),body2.tokens.begin(),body2.tokens.end());
              debug(body.getStringValue());
              return_node->branches.push_back(assembleLoop(expr,body,return_node,local::IFELSE));
              continue;
            }
            debug("I");
            //if
            TokenSegment expr = eatForHeader(ts);
            ts.next();
            TokenSegment body = ts.eatIndented(ts);
            return_node->branches.push_back(assembleLoop(expr,body,return_node,local::IF));
            continue;
        }
        if(ts.type() == WHILEKEYWORD)
        {
          //while
          TokenSegment expr = eatForHeader(ts);
          ts.next();
          TokenSegment body = ts.eatIndented(ts);
          return_node->branches.push_back(assembleLoop(expr,body,return_node,local::WHILE));
          continue;
        }
        if(ts.type() == SWITCHKEYWORD)
        {
          //switch
          TokenSegment expr = eatForHeader(ts);
          ts.next();
          TokenSegment body = ts.eatIndented(ts);
          return_node->branches.push_back(assembleLoop(expr,body,return_node,local::SWITCH));
          continue;
        }
        if(ts.type() == FORKEYWORD)
        {
          TokenSegment head = eatForHeader(ts);
          ts.next();
          TokenSegment body = ts.eatIndented(ts);
          return_node->branches.push_back(assembleLoop(head,body,return_node,local::FOR));
          continue;
        }
        if(ts.type() == FOREACHKEYWORD)
        {
          TokenSegment head = eatForHeader(ts);
          ts.next();
          TokenSegment body = ts.eatIndented(ts);
          return_node->branches.push_back(assembleLoop(head,body,return_node,local::FOREACH));
          continue;
        }
        if(ts.type() == CASEKEYWORD)
        {
          TokenSegment head = eatForHeader(ts);
          ts.next();
          TokenSegment body = ts.eatIndented(ts);
          return_node->branches.push_back(assembleLoop(head,body,return_node,local::CASE));
          continue;
        }
        if(ts.type() == FUNCTIONKEYWORD)
        {
          ts.next();
          TokenSegment type;
          type.push_back(ts.get());
          ts.next();
          TokenSegment name;
          name.push_back(ts.get());
          ts.next(); //skips open parentheses
          // eat until cparen reached
          TokenSegment parenlist = eatUntil({CPAREN},ts);
          ts.next();
          ts.next();
          TokenSegment body = ts.eatIndented(ts);
          return_node->branches.push_back(assembleFunc(type,name,parenlist,body,return_node));
          continue;
        }
	if(ts.type() == RETURNKEYWORD)
	{
    debug("R");
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
    debug("t" + std::to_string(ts.size()));
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

ASTNode* assembleVarInitA(TokenSegment ts,ASTNode* parent)
{
  VarDeclNode* return_node = new VarDeclNode(parent);
  return_node->isconst = false;
  for(ts.reset();!ts.end();ts.next())
  {
      if(ts.type() == ASYNCKEYWORD) fail("Invalid token!");
      if(ts.type() == CONSTKEYWORD) return_node->isconst = true;
  }
  ts.reset();
  return_node->vtype = ts.get().getValue(); //change it later to iterate over type
  ts.next();
  return_node->vname = ts.get().getValue();
  ts.next();
  ts.next();
  TokenSegment assignment;
  while(!ts.end())
  {
    assignment.push_back(ts.get());
    ts.next();
  }
  return_node->value = (ExprNode*) assembleExpr(assignment,return_node);
  return_node->vars_defined[{return_node->vname,return_node->vtype}] = true;
  return return_node;
}

ASTNode* assembleReturn(TokenSegment expr,ASTNode* parent)
{
	ReturnNode* return_node = new ReturnNode(parent);
	return_node->ret_expr = assembleExpr(expr,return_node);
	return return_node;
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
      if(local::getOperatorPriority(expr.type()) < local::getOperatorPriority(expr.at(op_iter).getType()))
      {
        op_iter = expr.current();
        return_node->operation = expr.type();
      }
      expr.next();
    }
      if(op_iter==0)
      {
        debug("Ret");
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
ASTNode* assembleLoop(TokenSegment head,TokenSegment body,ASTNode* parent,int which)
{
    switch(which)
    {
      case local::IF:{
        IfNode* return_node = new IfNode(parent);
        head.tokens.erase(head.tokens.begin(),head.tokens.begin()+1); //skips if keyword
        return_node->condition = (ExprNode*) assembleExpr(head,return_node);
        return_node->body = (CmdSeqNode*) assembleCmdSeq(body,return_node);
        return return_node;
      }
      case local::IFELSE:
      {
        IfElseNode* return_node = new IfElseNode(parent);
        TokenSegment ifbody;
        int scope = body.scope() - 1;
        while(body.type() != ELSEKEYWORD || body.scope() != scope)
        {
          debug("g");
          ifbody.push_back(body.get());
          body.next();
        }
        debug(std::to_string(body.current()));
        debug("IFCOMP:"+ifbody.getStringValue());
        return_node->ifcomponent = (IfNode*) assembleLoop(head,ifbody,return_node,local::IF);
        body.next();
        TokenSegment body2;
        while(!body.end())
        {
          body2.push_back(body.get());
          body.next();
        }
        return_node->body = (CmdSeqNode*) assembleCmdSeq(body2,return_node);
        return return_node;
      }
      case local::WHILE:
      {
        WhileNode* return_node = new WhileNode(parent);
        head.tokens.erase(head.tokens.begin(),head.tokens.begin()+1); //skips while keyword
        return_node->condition = (ExprNode*) assembleExpr(head,return_node);
        return_node->body = (CmdSeqNode*) assembleCmdSeq(body,return_node);
        return return_node;
      }
      case local::SWITCH:
      {
        SwitchNode* return_node = new SwitchNode(parent);
        head.tokens.erase(head.tokens.begin(),head.tokens.begin()+1); //skip switch keyword
        return_node->condition = (ExprNode*) assembleExpr(head,return_node);
        return_node->body = (CmdSeqNode*) assembleCmdSeq(body,return_node);
        return return_node;
      }
      case local::CASE:
      {
        CaseNode* return_node = new CaseNode(parent);
        head.tokens.erase(head.tokens.begin(),head.tokens.begin()+1); //skip case keyword
        return_node->condition = (ExprNode*) assembleExpr(head,return_node);
        return_node->body = (CmdSeqNode*) assembleCmdSeq(body,return_node);
        return return_node;
      }
      case local::FOR:
      {
        ForNode* return_node = new ForNode(parent);
        head.tokens.erase(head.tokens.begin(),head.tokens.begin()+1);
        TokenSegment initializer = eatVar(head); //This is an expression until we add variables
        head.next();
        TokenSegment condition = eatExpr(head);
        TokenSegment iterator; //Added Later
        return_node->initializer = (VarDeclNode*) assembleVarInitA(initializer,return_node);
        return_node->condition = (ExprNode*) assembleExpr(condition,return_node);
        return_node->body = (CmdSeqNode*) assembleCmdSeq(body,return_node);
        return return_node;
      }
      /*
      case local::FOREACH:
      {
        ForNode* return_node = new ForNode(parent);
        head.tokens.erase(head.tokens.begin(),head.tokens.begin()+1);
        TokenSegment initializer = head.eatExpr(); //This is an expression until we add variables
        ts.next();
        TokenSegment condition = head.eatExpr();
        ts.next();
        TokenSegment iterator; //Added Later
        return_node->initializer = (ExprNode*) assembleExpr(initializer,return_node);
        return_node->condition = (ExprNode*) assembleExpr(condition,return_node);
        return_node->body = (CmdSeqNode*) assembleCmdSeq(body,return_node);
        return return_node;
      }
      */ //To be added after variables
    }
    fail("Unidentified loop!");
}

ASTNode* assembleVarNode(TokenSegment ts,ASTNode* parent)
{
    ASTNode* return_node = new VarNode(parent);
    if(!working_tree->check(ts.getStringValue(),return_node))
    {
        debug("???");
    }
    return return_node;
}

ASTNode* assembleParenList(TokenSegment ts,ASTNode* parent)
{
    ParenList* return_node = new ParenList(parent);

    while(!ts.end())
    {
      TokenSegment piece;
      while(!ts.end() && ts.type() != COMMA)
      {
        piece.push_back(ts.get());
        ts.next();
      }
      return_node->branches.push_back(assembleVarInit(piece,return_node));
      ts.next();
    }
    return return_node;
}

ASTNode* assembleFunc(TokenSegment type,TokenSegment name,TokenSegment list,TokenSegment body,ASTNode* parent)
{
    FuncNode* return_node = new FuncNode(parent);
    return_node->type = type.getStringValue();
    return_node->name = name.getStringValue();
    return_node->arguments = (ParenList*) assembleParenList(list,return_node);
    return_node->body = (CmdSeqNode*) assembleCmdSeq(body,return_node);
    return return_node;
}

void ExprNode::assemble(){
    for(auto b : branches){ b->assemble(); debug(b->finished_result+","); }
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
    condition->assemble();
    body->assemble();
    finished_result = "if " + condition->finished_result + "{\n" + body->finished_result + "}";
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
    for(int i = 0; i < branches.size();i++){ branches[i]->assemble(); finished_result += branches[i]->finished_result;if(i+1 < branches.size()) finished_result += ',';}
}

void WhileNode::assemble()
{
  condition->assemble();
  body->assemble();
  finished_result = "while " + condition->finished_result + "{\n" + body->finished_result + "}";
}
void ForEachNode::assemble()
{

}
void SwitchNode::assemble()
{
  condition->assemble();
  body->assemble();
  finished_result = "switch " + condition->finished_result + "{\n" + body->finished_result + "}";
}
void IfElseNode::assemble()
{
  ifcomponent->assemble();
  body->assemble();
  finished_result = ifcomponent->finished_result + "\nelse{\n" + body->finished_result + "}";
}

void ForNode::assemble()
{
  initializer->assemble();
  condition->assemble();
  body->assemble();
  finished_result = "for (" + initializer->finished_result + ";" + condition->finished_result + ";){\n" + body->finished_result + "}";
}

void CaseNode::assemble()
{
  condition->assemble();
  body->assemble();
  finished_result = "case " + condition->finished_result + ":{\n" + body->finished_result + "}";
}

void VarDeclNode::assemble()
{
  value->assemble();
  if(isconst) finished_result = "const ";
  finished_result += vname + " " + vtype + " = " + value->finished_result;
}

void FuncNode::assemble()
{
  arguments->assemble();
  body->assemble();
  finished_result = type +" "+ name + "(" + arguments->finished_result + "){\n" + body->finished_result + "}";
}
