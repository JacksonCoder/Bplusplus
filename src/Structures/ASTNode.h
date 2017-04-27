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
        ~ASTNode();
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
        static ASTNode* assembleCmdSeq(TokenSegment,bool);
        static ASTNode* assembleReturnCmd(TokenSegment);
        static ASTNode* assembleFor(TokenSegment); //Work on implementation next
        static ASTNode* assembleForHeader(TokenSegment); //same here
        static ASTNode* assembleCmd(TokenSegment);
        static ASTNode* assembleFunc(TokenSegment);
        static ASTNode* assembleExpr(TokenSegment);
        static ASTNode* assembleVarDec(TokenSegment);
        static ASTNode* assembleKeyword(TokenSegment);
        static ASTNode* assembleListWithDelimiter(TokenSegment);
        static ASTNode* assembleFuncCall(TokenSegment);
        static ASTNode* assembleVarIdent(TokenSegment);
        static ASTNode* assembleFunctionHeader(TokenSegment);
        static ASTNode* assembleLoop(TokenSegment);
        static ASTNode* assembleLoopBody(TokenSegment);
        static ASTNode* assembleLoopHeader(TokenSegment);
        static ASTNode* assembleIf(TokenSegment);
        static ASTNode* assembleIfHeader(TokenSegment);
        static ASTNode* assembleVarInit(TokenSegment);
        static bool checkIdentification(TokenSegment,Type);
        void print_tree(int);
        std::map<std::string,ASTNode*> data;
        std::string meta;
    protected:
    private:

        //implement type system ( and add vectors )

        Type type;
        std::string token;
        std::string result;
};
//#include "ASTTree.h"
#endif // ASTNode_H
