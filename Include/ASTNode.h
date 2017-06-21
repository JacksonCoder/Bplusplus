#ifndef ASTNode_H
#define ASTNode_H
#include "tokenType.h"
#include "File.h"
#include "ASTTree.h"
#include "TokenSegment.h"
#include "../src/Libraries/shared.h"
#include "parsing.h"
class ASTTree;
class NodeData {
    public:
        std::map<std::string,std::string> data;
};
class ASTNode
{
    public:
        ASTNode(ASTNode*);
        ~ASTNode();
        Type type;
        unsigned int line;
        TokenSegment component;
        std::string str_component;
        std::string finished_result;
        NodeData node_data;
        std::vector<ASTNode*> branches;
        virtual void assemble();
        std::map<std::string,std::vector<std::string> > msg; //For 0.6
        std::map<std::pair<std::string,std::string>,bool> vars_defined;
	ASTNode* parent;
};
class VarInitNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class ExprNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class IfNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class CmdSeqNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class ReturnNode : public ASTNode { //work on constructor implementation another time
    public:
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
	using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
};
class EndpointNode : public ASTNode {
    public:
	using ASTNode::ASTNode;
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
    using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
}
class FuncNode : public ASTNode {
    public:
    using ASTNode::ASTNode;
        virtual void assemble();
        static bool is(TokenSegment);
}
#endif // ASTNode_H
