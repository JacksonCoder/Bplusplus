#ifndef TVAR_H
#define TVAR_H
#include "../Libraries/shared.h"
#include "../Compiler/fail.h"
class TVar
{
    public:
        TVar();
        ~TVar();
        TVar(std::string);
        TVar(std::string,std::string);
        std::string getType() {     return type; }
        void setType(std::string val) { type = val; }
        std::string getName() { return name; }
        void setName(std::string val) { name = val; }
    protected:
    private:
        std::string type;
        std::string name;
};

#endif // TVAR_H
