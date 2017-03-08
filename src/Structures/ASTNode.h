#ifndef ASTNode_H
#define ASTNode_H
#include <regex>
#include "tokenType.h"
#include "Error.h"
#include "File.h"
#include "TVar.h"
#include "ASTTree.h"
class ASTTree;
class ASTNode
{
    public:
        ASTNode* getPreviousSibling();
        ASTNode* getNextSibling();
        ASTNode(Type t);
        ~ASTNode();
        ASTNode(std::string);
        ASTNode(std::string,ASTNode*);
        ASTNode(std::string s,Type t): token(s),type(t) {}
        Type getType(){ return type;}
        Error assemble();
        void assembleSubNodes(ASTTree&);
        Type determineType(std::string);
        std::string getToken(){ return token;}
        std::string getResult(){ return result;}
        void setToken(std::string t){token=t;}
        std::vector<ASTNode*> branches;
        ASTNode* parent;
        void setTokenMatches(std::smatch);
        bool isArglist(std::string);
        bool isFunction(std::string);
        bool isFunctionA(std::string);
        bool isFunctionT(std::string);
        bool isFunctionAT(std::string);
        bool isVarInit(std::string);
        bool isVarDecl(std::string);
        bool isImportStatement(std::string);
        bool isReturnStatement(std::string);
        bool isVarInitWithArguments(std::string);
        std::map<std::string,ASTNode*> data;
        std::map<std::string,ASTNode*> metaData;
        std::map<std::string,TVar*> variables;
    protected:
    private:

        //implement type system ( and add vectors )

        Type type;
        std::vector<std::string> tokenmatches;
        std::string token;
        std::string result;
};
//#include "ASTTree.h"
#endif // ASTNode_H
