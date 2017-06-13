#ifndef ASTTree_H
#define ASTTree_H
#include "ASTNode.h"
#include "tokenType.h"
#include "TokenSegment.h"
class ASTNode;
class ASTTree
{
    public:
        /** Default constructor */
        ASTTree();
        /** Default destructor */
        ~ASTTree();
        void setup();
        ASTNode* getRoot(){ return root;}
        void addNodeTo(ASTNode*,std::string);
        void metaSetup();
        void varAdd(std::string,std::string,ASTNode*);
        std::string type(std::string,ASTNode*);
        bool varSearch(std::string,ASTNode*);
        void build(TokenSegment);
        void mapVar(std::string,std::string,ASTNode*);
        bool check(std::string,ASTNode*);
    protected:
    private:
         ASTNode* root;
         std::map<std::pair<std::string,ASTNode*>,std::string> vars;
};

#endif // ASTTree_H
