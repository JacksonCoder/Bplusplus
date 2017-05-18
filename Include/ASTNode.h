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
class Scope {
    public:
        bool inFunction;
        bool inClass;
        unsigned int loopsIn;
};
class ASTNode
{
    public:
        ASTNode();
        ~ASTNode();
        Type type;
        Scope scope;
        unsigned int line;
        TokenSegment component;
        std::string str_component;
        std::string finished_result;
        NodeData node_data;
        std::vector<ASTNode*> branches;
        virtual void assemble();
};
class VarNode : public ASTNode {
    public:
        virtual void assemble();
        static bool is(TokenSegment);
};
class ExprNode : public ASTNode {
    public:
        virtual void assemble();
        static bool is(TokenSegment);
};
class IfNode : public ASTNode {
    public:
        virtual void assemble();
        static bool is(TokenSegment);
};
class CmdSeqNode : public ASTNode {
    public:
        virtual void assemble();
        static bool is(TokenSegment);
};
class CmdNode : public ASTNode {
    public:
        virtual void assemble();
        static bool is(TokenSegment);
};
class ReturnNode : public ASTNode {
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
        virtual void assemble();
        static bool is(TokenSegment);
};
#endif // ASTNode_H
