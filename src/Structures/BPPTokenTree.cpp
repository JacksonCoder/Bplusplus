#include "BPPTokenTree.h"
#include "Libraries/shared.h"
#include "Structures/tokenType.h"
BPPTokenTree::BPPTokenTree()
{
    root = new BPPTNode(ROOT);
}

void BPPTokenTree::setup()
{
    root->assembleSubNodes();
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
        if(b->getType()==t) return b;
        if(searchBranches(b,t)!=nullptr) return searchBranches(b,t);
    }
    return nullptr;
}

void determineTypeOfValue(std::string value)
{
    //work on later
}
void BPPTokenTree::metaSetup()
{
    BPPTNode* astIterator = root;
    for(auto b : astIterator->branches)
    {
        /*
        if(b->getType() == FUNCTION || b->getType() == FUNCTIONA)
        { //Later, I need to add parsing features to find the type of this function
            std::cout<<"Let's begin"<<std::endl;
            //BPPTNode* returnKeyword = searchBranches(b,RETURN);

        }
        */
    }
}
