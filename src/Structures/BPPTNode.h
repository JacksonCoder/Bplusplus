#ifndef BPPTNODE_H
#define BPPTNODE_H
#include <regex>
#include "Structures/tokenType.h"
#include "Structures/BPPError.h"
#include "Structures/BPPFile.h"
#include "BPPTVar.h"
class BPPTokenTree;
class BPPTNode
{
    public:
        BPPTNode* getPreviousSibling();
        BPPTNode* getNextSibling();
        BPPTNode(Type t);
        ~BPPTNode();
        BPPTNode(std::string);
        BPPTNode(std::string s,Type t): token(s),type(t) {}
        Type getType(){ return type;}
        BPPError assemble();
        void assembleSubNodes(BPPTokenTree&);
        Type determineType(std::string);
        std::string getToken(){ return token;}
        std::string getResult(){ return result;}
        std::vector<BPPTNode*> branches; //!< Member variable "branches"
        BPPTNode* parent;
        void setTokenMatches(std::smatch);
        std::vector<BPPTVar> variables;
    protected:
    private:

        //implement type system ( and add vectors )

        Type type;
        std::smatch tokenmatches;
        std::map<std::string,BPPTNode*> data; //!< Member variable "data"
        std::map<std::string,BPPTNode*> metaData; //!< Member variable "metaData"
        std::string token;
        std::string result;
};
#include "BPPTokenTree.h"
#endif // BPPTNODE_H
