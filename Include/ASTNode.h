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
        ASTNode(Type); //<-Constructor
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
        VarNode(): ASTNode(VARINIT) {}
        virtual void assemble();
};
class ExprNode : ASTNode {
    public:
        virtual void assemble();
};
class IfNode : ASTNode {
    public:
        virtual void assemble();
};
class CmdSeqNode : ASTNode {
    public:
        virtual void assemble();
};
class CmdNode : ASTNode {
    public:
        virtual void assemble();
};
#endif // ASTNode_H
