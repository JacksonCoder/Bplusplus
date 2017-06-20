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
    root = assembleTop(tokenlist,this); //top level function that sets the tree up
    //root->finish(); //resolves compile-time dependencies from variable inferencing
    root->assemble();
    std::cout<<"Finished result:\n"<<root->finished_result<<std::endl;
}
void ASTTree::mapVar(std::string name,std::string type,ASTNode* scope)
{
    vars[{name,scope}] = type;
}

bool r_check(ASTTree* tree,std::string name,ASTNode* location,unsigned int listloc)
{
	/*
	if (location->parent == NULL)
	{
		return false;
	}
	else if (tree->vars.find({name,location}) != vars.end())
	{
		return true;
	}


	listloc //work on this
	return r_check(tree,name,location->parent,listloc);
	*/
	return false;
}

bool ASTTree::check(std::string name,ASTNode* location)
{
	return r_check(this,name,location,0);
}
