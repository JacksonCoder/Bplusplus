#ifndef BPPTNODE_H
#define BPPTNODE_H
#include <regex>
#include "Structures/tokenType.h"
#include "Structures/BPPError.h"
#include "Structures/BPPFile.h"
#include "BPPTVar.h"
#include "BPPTokenTree.h"
class BPPTokenTree;
class BPPTNode
{
    public:
        BPPTNode* getPreviousSibling();
        BPPTNode* getNextSibling();
        BPPTNode(Type t);
        ~BPPTNode();
        BPPTNode(std::string);
        BPPTNode(std::string,BPPTNode*);
        BPPTNode(std::string s,Type t): token(s),type(t) {}
        Type getType(){ return type;}
        BPPError assemble();
        void assembleSubNodes(BPPTokenTree&);
        Type determineType(std::string);
        std::string getToken(){ return token;}
        std::string getResult(){ return result;}
        void setToken(std::string t){token=t;}
        std::vector<BPPTNode*> branches;
        BPPTNode* parent;
        void setTokenMatches(std::smatch);
        bool isArglist(std::string);
        std::map<std::string,BPPTNode*> data;
        std::map<std::string,BPPTNode*> metaData;
        std::map<std::string,BPPTVar*> variables;
    protected:
    private:

        //implement type system ( and add vectors )

        Type type;
        std::vector<std::string> tokenmatches;
        std::string token;
        std::string result;
};
//#include "BPPTokenTree.h"
#endif // BPPTNODE_H
