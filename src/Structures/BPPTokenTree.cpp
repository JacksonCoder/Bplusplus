#include "BPPTokenTree.h"
#include "Libraries/shared.h"
#include "Structures/tokenType.h"
BPPTokenTree::BPPTokenTree()
{
    root = new BPPTNode(ROOT);
}

void BPPTokenTree::setup()
{
    root->assembleSubNodes(*this);
    std::cout<<"Variables:"<<variables.size()<<std::endl;
    for(std::map<std::string,BPPTVar*>::iterator it=variables.begin(); it!=variables.end(); ++it) std::cout<<it->second->getName()<<std::endl<<it->second->getType()<<std::endl;
}

void BPPTokenTree::addNodeTo(BPPTNode* attachTo,std::string data)
{
    attachTo->branches.push_back(new BPPTNode(data));
    attachTo->branches.at(attachTo->branches.size()-1)->parent = attachTo;
}
BPPTokenTree::~BPPTokenTree()
{
    delete root;
}



BPPTNode* searchBranches(BPPTNode* root,Type t)
{
    if(root->branches.size() > 0) for(auto b : root->branches)
    {
        if(b->getType()==t){return b;}
        if(searchBranches(b,t)!=nullptr) return searchBranches(b,t);
    }
    return nullptr;
}

void BPPTokenTree::varAdd(std::string name, std::string type)
{
    variables[name] = new BPPTVar(name,type);
}

void determineTypeOfValue(std::string value)
{
    //work on later
}
void BPPTokenTree::metaSetup()
{
    BPPTNode* astIterator = root;
    std::cout<<root->branches.size()<<std::endl;
    for(auto b : astIterator->branches)
    {
        if(b->getType() == FUNCTION || b->getType() == FUNCTIONA)
        { //Later, I need to add parsing features to find the type of this function
            BPPTNode* returnKeyword = searchBranches(b,RETURN);
        }

    }
}
