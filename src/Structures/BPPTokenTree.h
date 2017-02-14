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
         BPPTNode* root; //!< Member variable "root"
         BPPError error;
};

#endif // BPPTOKENTREE_H
