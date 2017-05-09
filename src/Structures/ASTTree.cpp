#include "../../Include/ASTTree.h"

ASTTree::ASTTree()
{

}

ASTTree::~ASTTree()
{
    delete root;
}

void ASTTree::build(TokenSegment tokenlist)
{
    //top level recursive assembly
    std::cout<<"I am here!";
    root = ASTNode::assembleTop(tokenlist); //top level function that sets the tree up
    //root->finish(); //resolves compile-time dependencies from variable inferencing
    root->assemble();
}