#include "ASTTree.h"

ASTTree::ASTTree()
{

}

ASTTree::~ASTTree()
{
    delete root;
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