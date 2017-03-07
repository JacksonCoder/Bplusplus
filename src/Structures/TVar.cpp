#include "TVar.h"

TVar::TVar()
{
    //ctor
}

TVar::TVar(std::string s) : name(s)
{}

TVar::TVar(std::string name,std::string type) : name(name), type(type)
{}

TVar::~TVar()
{
    //dtor
}
