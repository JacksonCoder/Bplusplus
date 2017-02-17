#ifndef BPPTOKENTREE_H
#define BPPTOKENTREE_H
#include "Structures/BPPTNode.h"
#include "Structures/BPPError.h"
class BPPTokenTree
{
    public:
        /** Default constructor */
        BPPTokenTree();
        /** Default destructor */
        ~BPPTokenTree();
        void setup();
        BPPTNode* getRoot(){ return root;}
        bool isError(){ return error.errorExists();}
        BPPError& getError(){ return error;}
        void addNodeTo(BPPTNode*,std::string);
        void metaSetup();
    protected:
    private:
        /*
        std::vector<std::string,BPPTVar*> variables; <- for version 0.2
        std::vector<std::String,BPPTVar*> functions; <- for version 0.2
        */
         BPPTNode* root; //!< Member variable "root"
         BPPError error;
};

#endif // BPPTOKENTREE_H
