#include "../../Include/ASTNode.h"

ASTNode::ASTNode()
{}
ASTNode::~ASTNode()
{
    for(int branch_i = 0; branch_i < branches.size();branch_i++)
    {
        delete branches[branch_i];
    }
}

bool IfNode::is(TokenSegment ts)
{
    return ts.tokens[0].getType() == IFKEYWORD;
}
bool ForNode::is(TokenSegment ts)
{
    return ts.tokens[0].getType() == FORKEYWORD;
}