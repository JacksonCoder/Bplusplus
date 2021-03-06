#include "../../Include/ASTNode.h"

ASTNode::ASTNode(ASTNode* parent)
{
	this->context.parent = parent;
}

ASTNode::~ASTNode()
{
    for(int branch_i = 0; branch_i < branches.size();branch_i++)
    {
        delete branches[branch_i];
    }
}

bool IfNode::is(TokenSegment ts)
{
    return ts.at(0).getType() == IFKEYWORD && ts.size() > 1;
}

bool ForNode::is(TokenSegment ts)
{
    return ts.at(0).getType() == FORKEYWORD && ts.size() >= 4;
}

bool VarNode::is(TokenSegment ts)
{
    return ts.size() >= 2; //Add more later
}
bool VarInitNode::is(TokenSegment ts)
{
	return ts.size() >= 3;
}
bool ParenList::is(TokenSegment ts)
{
    return true;
}