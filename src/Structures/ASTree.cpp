#include "ASTTree.h"

ASTTree::ASTTree()
{

}

std::string ASTTree::type(std::string searcher,ASTNode* scope)
{
    if(scope->variables.count(searcher))
    return scope->variables[searcher]->getType();
}


bool ASTTree::varSearch(std::string searcher, ASTNode* scope)
{
    if(scope->variables.count(searcher)) return true;
    return false;
}

void ASTTree::setup()
{
    root->assembleSubNodes(*this);
}

void ASTTree::addSegmentBranch(ASTNode* attachTo,TokenSegment ts)
{
    attachTo->branches.push_back(new ASTNode(ts));
    attachTo->branches.at(attachTo->branches.size()-1)->parent = attachTo;
}
ASTTree::~ASTTree()
{
    delete root;
}



ASTNode* searchBranches(ASTNode* root,Type t)
{
    if(root->branches.size() > 0) for(auto b : root->branches)
    {
        if(b->getType()==t){return b;}
        if(searchBranches(b,t)!=nullptr){return searchBranches(b,t);}
    }
    return nullptr;
}

void ASTTree::varAdd(std::string name, std::string type,ASTNode* attacher)
{
    attacher->variables[name] = new TVar(name,type);
}



enum IndentationType
{
    SPACES,
    TABS
};
/*
IndentationType getIndentationType(File* bfile)
{
    for(int lines_i = 0; lines_i < bfile->lines.size();lines_i++)
    {
        if(bfile->lines[lines_i][0] == ' ')
        {
            return SPACES;
        }
        if(bfile->lines[lines_i][0] == '\t')
        {
            return TABS;
        }
    }
    return SPACES; //default
}
*/
void ASTTree::build(TokenSegment tokenlist)
{
    //top level recursive assembly
    root = ASTNode::assembleTop(tokenlist); //top level function that sets the tree up
    root.finish(); //resolves compile-time dependencies from variable inferencing
}