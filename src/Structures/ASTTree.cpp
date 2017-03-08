#include "ASTTree.h"

ASTTree::ASTTree()
{
    root = new ASTNode(ROOT);
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
    std::cout<<"Variables:"<<variables.size()<<std::endl;
    for(std::map<std::string,TVar*>::iterator it=variables.begin(); it!=variables.end(); ++it) std::cout<<it->second->getName()<<std::endl<<it->second->getType()<<std::endl;
}

void ASTTree::addNodeTo(ASTNode* attachTo,std::string data)
{
    attachTo->branches.push_back(new ASTNode(data));
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

void ASTTree::metaSetup()
{
    ASTNode* astIterator = root;
    for(auto b : astIterator->branches)
    {
        if(b->getType() == FUNCTION || b->getType() == FUNCTIONA)
        { //Later, I need to add parsing features to find the type of this function
            ASTNode* returnKeyword = searchBranches(b,RETURN);
            std::string type = returnKeyword->data["returntype"]->getToken();
            b->metaData["type"]->setToken(type);
        }

    }
}
