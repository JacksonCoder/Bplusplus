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
    ASTNode* attacher = root;
    int scopelevel = 0;
    bool processing = true;
    unsigned int line=0,numberofindents = 0;
    while(processing){
        
        numberofindents = tokenlist.countAmountOfTokensBetweenTermination(INDENT,line);
        TokenSegment line = tokenlist.getLine(line);
        if(numberofindents > scopelevel){

            scopelevel++;

            if(i>0){ attacher = attacher->branches[attacher->branches.size()-1]; }
            else
            {
                fail("Error: unexpected lack of indentation!");
            }
            if(numberofindents > scopelevel)
                {
                fail("Error: unexpected lack of indentation!");
            }
        }
        if(numberofindents < scopelevel){

            scopelevel--;

            if(i>0){ attacher = attacher->parent; }
            else
            {
                fail("Error: unexpected lack of indentation!");
            }
            if(numberofindents > scopelevel)
                {
                fail("Error: unexpected lack of indentation!");
            }
        }
        this->addSegmentBranch(attacher,line);
        line++;
    }
    setup();
    metaSetup();
    return returnValue;
}

void preParseContents(File* bfile)
{

}