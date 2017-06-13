#ifndef PARSING_H
#define PARSING_H
#include "../src/Libraries/shared.h"
#include "ASTNode.h"
#include "ASTTree.h"
class ASTTree;
class ASTNode;
ASTNode* assembleTop(TokenSegment,ASTNode* parent);
ASTNode* assembleCmdSeq(TokenSegment,ASTNode* parent);
ASTNode* assembleReturnCmd(TokenSegment,ASTNode* parent);
ASTNode* assembleFor(TokenSegment,ASTNode* parent);
ASTNode* assembleFunc(TokenSegment,ASTNode* parent);
ASTNode* assembleExpr(TokenSegment,ASTNode* parent);
ASTNode* assembleVarDec(TokenSegment,ASTNode* parent);
ASTNode* assembleKeyword(TokenSegment,ASTNode* parent);
ASTNode* assembleListWithDelimiter(TokenSegment,ASTNode* parent);
ASTNode* assembleFuncCall(TokenSegment,ASTNode* parent);
ASTNode* assembleVarIdent(TokenSegment,ASTNode* parent);
ASTNode* assembleFunctionHeader(TokenSegment,ASTNode* parent);
ASTNode* assembleLoop(TokenSegment,ASTNode* parent);
ASTNode* assembleLoopBody(TokenSegment,ASTNode* parent);
ASTNode* assembleLoopHeader(TokenSegment,ASTNode* parent);
ASTNode* assembleIf(TokenSegment,ASTNode* parent);
ASTNode* assembleIfHeader(TokenSegment,ASTNode* parent);
ASTNode* assembleVarInit(TokenSegment,ASTNode* parent);
ASTNode* assembleEndpoint(TokenSegment,ASTNode* parent);
ASTNode* assembleVarInitA(TokenSegment,ASTNode* parent);
ASTNode* assembleVarAssign(TokenSegment,ASTNode* parent);
ASTNode* assembleVarNode(TokenSegment,ASTNode* parent);
ASTTree* working_tree;
#endif //PARSING_H
