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
bool ASTTree::check(std::string name,ASTNode* location)
{
	while(location->parent!=NULL)
	{;
		if(vars.find({name,location})!=vars.end())
		{
			return true;
		}
		location = location->parent;
	}
	return false;
}
