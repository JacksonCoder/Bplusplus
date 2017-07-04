#ifndef PARSING_H
#define PARSING_H
#include "../src/Libraries/shared.h"
#include "ASTNode.h"
#include "ASTTree.h"
class ASTTree;
class ASTNode;
ASTNode* assembleTop(TokenSegment,ASTTree*);
ASTNode* assembleCmdSeq(TokenSegment,ASTNode* parent);
ASTNode* assembleReturnCmd(TokenSegment,ASTNode* parent);
ASTNode* assembleFor(TokenSegment,ASTNode* parent);
ASTNode* assembleExpr(TokenSegment,ASTNode* parent);
ASTNode* assembleIf(TokenSegment,TokenSegment,ASTNode* parent);
ASTNode* assembleIfHeader(TokenSegment,ASTNode* parent);
ASTNode* assembleVarInit(TokenSegment,ASTNode* parent);
ASTNode* assembleEndpoint(TokenSegment,ASTNode* parent);
ASTNode* assembleVarInitA(TokenSegment,ASTNode* parent);
ASTNode* assembleVarAssign(TokenSegment,ASTNode* parent);
ASTNode* assembleVarNode(TokenSegment,ASTNode* parent);
ASTNode* assembleParenList(TokenSegment,ASTNode* parent);
ASTNode* assembleFunc(TokenSegment,ASTNode* parent);
#endif //PARSING_H
