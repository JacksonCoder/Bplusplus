#include "../src/Libraries/shared.h"
#include "ASTNode.h"
class ASTNode;
ASTNode* assembleTop(TokenSegment);
ASTNode* assembleCmdSeq(TokenSegment,bool);
ASTNode* assembleReturnCmd(TokenSegment);
ASTNode* assembleFor(TokenSegment); //Work on implementation next
ASTNode* assembleForHeader(TokenSegment); //same here
ASTNode* assembleCmd(TokenSegment);
ASTNode* assembleFunc(TokenSegment);
ASTNode* assembleExpr(TokenSegment);
ASTNode* assembleVarDec(TokenSegment);
ASTNode* assembleKeyword(TokenSegment);
ASTNode* assembleListWithDelimiter(TokenSegment);
ASTNode* assembleFuncCall(TokenSegment);
ASTNode* assembleVarIdent(TokenSegment);
ASTNode* assembleFunctionHeader(TokenSegment);
ASTNode* assembleLoop(TokenSegment);
ASTNode* assembleLoopBody(TokenSegment);
ASTNode* assembleLoopHeader(TokenSegment);
ASTNode* assembleIf(TokenSegment);
ASTNode* assembleIfHeader(TokenSegment);
ASTNode* assembleVarInit(TokenSegment);
bool checkIdentification(TokenSegment);