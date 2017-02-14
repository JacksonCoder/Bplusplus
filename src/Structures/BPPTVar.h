#ifndef BPPTVAR_H
#define BPPTVAR_H
#include "Libraries/shared.h"

class BPPTVar
{
    public:
        BPPTVar();
        ~BPPTVar();
        BPPTVar(std::string);
        BPPTVar(std::string,std::string);
        std::string getType() { return type; }
        void setType(std::string val) { type = val; }
        std::string getName() { return name; }
        void setName(std::string val) { name = val; }
    protected:
    private:
        std::string type;
        std::string name;
};

#endif // BPPTVAR_H
