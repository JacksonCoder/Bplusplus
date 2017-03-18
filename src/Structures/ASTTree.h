#ifndef ASTTree_H
#define ASTTree_H
#include "ASTNode.h"
#include "tokenType.h"
#include "Error.h"
#include "TVar.h"
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
        bool isError(){ return error.errorExists();}
        Error& getError(){ return error;}
        void addNodeTo(ASTNode*,std::string);
        void metaSetup();
        void varAdd(std::string,std::string,ASTNode*);
        std::string type(std::string,ASTNode*);
        bool varSearch(std::string,ASTNode*);
        void build(TokenSegment);
    protected:
    private:

        std::map<std::string,TVar*> variables; //<- for version 0.2
        std::map<std::string,TVar*> functions; //<- for version 0.2

         ASTNode* root; //!< Member variable "root"
         Error error;
};

#endif // ASTTree_H
