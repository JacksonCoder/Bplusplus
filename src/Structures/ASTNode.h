#ifndef ASTNode_H
#define ASTNode_H
#include <regex>
#include "tokenType.h"
#include "Error.h"
#include "File.h"
#include "TVar.h"
#include "ASTTree.h"
#include "TokenSegment.h"
class ASTTree;
class ASTNode
{
    public:
        //ASTNode* getPreviousSibling();
        //ASTNode* getNextSibling();
        ASTNode(Type t);
        ~ASTNode();
        ASTNode(TokenSegment);
        ASTNode(TokenSegment,ASTNode*);
        ASTNode(TokenSegment ts,Type t): tokenseg(ts),type(t) {token=tokenseg.getStringValue();}
        ASTNode(std::string,ASTNode*);
        ASTNode(std::string);
        Type getType(){ return type;}
        Error assemble();
        void assembleSubNodes(ASTTree&);
        Type determineType(TokenSegment);
        std::string getToken(){ return token;}
        TokenSegment getTokenSegment() { return tokenseg; }
        std::string getResult(){ return result;}
        void setToken(std::string t){token=t;}
        std::vector<ASTNode*> branches;
        ASTNode* parent;
        void setTokenMatches(std::smatch);
        bool isArglist(TokenSegment);
        bool isFunction(TokenSegment);
        bool isFunctionA(TokenSegment);
        bool isFunctionT(TokenSegment);
        bool isFunctionAT(TokenSegment);
        bool isVarInit(TokenSegment);
        bool isVarDecl(TokenSegment);
        bool isImportStatement(TokenSegment);
        bool isReturnStatement(TokenSegment);
        bool isVarInitWithArguments(TokenSegment);
        std::map<std::string,ASTNode*> data;
        std::map<std::string,ASTNode*> metaData;
        std::map<std::string,TVar*> variables;
    protected:
    private:

        //implement type system ( and add vectors )

        Type type;
        std::vector<std::string> tokenmatches;
        TokenSegment tokenseg;
        std::string token;
        std::string result;
};
//#include "ASTTree.h"
#endif // ASTNode_H
