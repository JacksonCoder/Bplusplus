#ifndef PARSING_H
#define PARSING_H
#include "../src/Libraries/shared.h"
#include "ASTNode.h"
#include "ASTTree.h"
class ASTTree;
class ASTNode;
ASTNode* assembleTop(TokenSegment,ASTTree*);
ASTNode* assembleCmdSeq(TokenSegment,ASTNode* parent);
ASTNode* assembleReturn(TokenSegment,ASTNode* parent);
ASTNode* assembleFor(TokenSegment,ASTNode* parent);
ASTNode* assembleExpr(TokenSegment,ASTNode* parent);
ASTNode* assembleLoop(TokenSegment,TokenSegment,ASTNode* parent,int which);
ASTNode* assembleVarInit(TokenSegment,ASTNode* parent);
ASTNode* assembleEndpoint(TokenSegment,ASTNode* parent);
ASTNode* assembleVarInitA(TokenSegment,ASTNode* parent);
ASTNode* assembleVarAssign(TokenSegment,ASTNode* parent);
ASTNode* assembleVarNode(TokenSegment,ASTNode* parent);
ASTNode* assembleParenList(TokenSegment,ASTNode* parent);
ASTNode* assembleFunc(TokenSegment,TokenSegment,TokenSegment,TokenSegment,ASTNode* parent);
ASTNode* assembleInclude(TokenSegment,bool,ASTNode*);
#endif //PARSING_H
