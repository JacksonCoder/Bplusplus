#ifndef BPPTOKENTREE_H
#define BPPTOKENTREE_H

#include "Structures/BPPError.h"
#include "BPPTNode.h"
#include "BPPTVar.h"
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
        void varAdd(std::string,std::string);
        std::string type(std::string);
        bool varSearch(std::string);
    protected:
    private:

        std::map<std::string,BPPTVar*> variables; //<- for version 0.2
        std::map<std::string,BPPTVar*> functions; //<- for version 0.2

         BPPTNode* root; //!< Member variable "root"
         BPPError error;
};

#endif // BPPTOKENTREE_H
