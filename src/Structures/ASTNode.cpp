#include "../../Include/ASTNode.h"

ASTNode::ASTNode(Type t): type(t)
{}
ASTNode::~ASTNode()
{
    /*
    for(int branch_i = 0; branch_i < branches.size();branch_i++)
    {
        delete branches[branch_i];
    }
    */
}
