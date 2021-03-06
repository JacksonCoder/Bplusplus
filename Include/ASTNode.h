#ifndef ASTNode_H
#define ASTNode_H
#include "tokenType.h"
#include "File.h"
#include "ASTTree.h"
#include "TokenSegment.h"
#include "../src/Libraries/shared.h"
#include "parsing.h"
class ASTTree;
class ExprNode;
class Context
{
    public:
    unsigned int linenum;
    ASTNode* parent;
};

class ASTNode
{
    public:
        ASTNode(ASTNode*);
        ~ASTNode();
        unsigned int line;
        std::string finished_result;
        Context context;
        std::vector<ASTNode*> branches;
        virtual void assemble();
        std::map<std::pair<std::string,std::string>,bool> vars_defined;
};
class VarInitNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
	std::string vtype;
	std::string vname;
	bool isconst;
        virtual void assemble();
        static bool is(TokenSegment);
};
class VarDeclNode : public ASTNode {
public:
  bool isconst;
  std::string vname;
  std::string vtype;
  ExprNode* value;
  using ASTNode::ASTNode;
  virtual void assemble();
  static bool is(TokenSegment);
};
class ExprNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
	unsigned int operation = 0;
	unsigned int type = 0;
  std::string endpoint_val;
        virtual void assemble();
        static bool is(TokenSegment);
};
class CmdSeqNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class IfNode : public ASTNode {
    public:
      ExprNode* condition;
      CmdSeqNode* body;
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class ReturnNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
	ASTNode* ret_expr = NULL;
        virtual void assemble();
        static bool is(TokenSegment);
};
class DeclNode : public ASTNode {
    public:
        virtual void assemble();
        static bool is(TokenSegment);
};
class ForNode : public ASTNode {
    public:
      VarDeclNode* initializer; //DeclNode later
      ExprNode* condition;
      //CmdNode* iterator;
      CmdSeqNode* body;
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class WhileNode : public ASTNode {
    public:
      ExprNode* condition;
      CmdSeqNode* body;
	    using ASTNode::ASTNode;
      virtual void assemble();
      static bool is(TokenSegment);
};
class ForEachNode : public ASTNode {
    public:
      //VarNode* iterator;
      ExprNode* container;
      CmdSeqNode* body;
	    using ASTNode::ASTNode;
      virtual void assemble();
      static bool is(TokenSegment);
};
class CaseNode : public ASTNode {
public:
  ExprNode* condition;
  CmdSeqNode* body;
  using ASTNode::ASTNode;
  virtual void assemble();
  static bool is(TokenSegment);
};
class SwitchNode : public ASTNode {
    public:
      ExprNode* condition;
      CmdSeqNode* body;
	    using ASTNode::ASTNode;
      virtual void assemble();
      static bool is(TokenSegment);
};
class IfElseNode : public ASTNode {
    public:
      IfNode* ifcomponent;
      CmdSeqNode* body;
	    using ASTNode::ASTNode;
      virtual void assemble();
      static bool is(TokenSegment);
};
class EndpointNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
    std::string string_comp;
        virtual void assemble();
        static bool is(TokenSegment){return true;} //for now
};
class ForHeaderNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class VarAssignNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class VarInitANode : public ASTNode {
    public:
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class VarNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class ParenList : public ASTNode {
    public:
      std::vector<ASTNode*> parameters;
    using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class FuncNode : public ASTNode {
    public:
    std::string name;
    std::string type; //create typenode later
    ParenList* arguments;
    CmdSeqNode* body;
    using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class IncludeNode : public ASTNode {
public:
  using ASTNode::ASTNode;
  std::string filename;
  virtual void assemble();
  static bool is(TokenSegment);
};
class IncludeLocalNode : public ASTNode {
public:
  using ASTNode::ASTNode;
  std::string filename;
  virtual void assemble();
  static bool is(TokenSegment);
};
#endif // ASTNode_H
