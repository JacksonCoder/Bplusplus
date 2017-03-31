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
        ASTNode(Type); //<-Constructor
        Type getType(){ return type;}
        void assemble();
        void assembleSubNodes(ASTTree&);
        Type determineType(TokenSegment);
        std::string getToken(){ return token;}
        std::string getResult(){ return result;}
        void setToken(std::string t){token=t;}
        std::vector<ASTNode*> branches;
        ASTNode* parent;
        void setTokenMatches(std::smatch);
        static ASTNode* assembleTop(TokenSegment);
        static ASTNode* assembleCmdSeq(TokenSegment);
        static ASTNode* assembleCmd(TokenSegment);
        static ASTNode* assembleFunc(TokenSegment);
        static ASTNode* assembleExpr(TokenSegment);
        static ASTNode* assembleVarDec(TokenSegment);
        static ASTNode* assembleKeyword(TokenSegment);
        static ASTNode* assembleListWithDelimiter(TokenSegment);
        static ASTNode* assembleFuncCall(TokenSegment);
        static ASTNode* assembleVarIdent(TokenSegment);
        static ASTNode* assembleFunctionHeader(TokenSegment);
        static bool checkIdentification(TokenSegment,Type);
        std::map<std::string,ASTNode*> data;
        std::map<std::string,ASTNode*> metaData;
        std::map<std::string,TVar*> variables;
    protected:
    private:

        //implement type system ( and add vectors )

        Type type;
        std::string token;
        std::string result;
};
//#include "ASTTree.h"
#endif // ASTNode_H
