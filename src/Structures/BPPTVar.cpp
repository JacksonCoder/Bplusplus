#include "BPPTVar.h"

BPPTVar::BPPTVar()
{
    //ctor
}

BPPTVar::BPPTVar(std::string s) : name(s)
{}

BPPTVar::BPPTVar(std::string name,std::string type) : name(name), type(type)
{}

BPPTVar::~BPPTVar()
{
    //dtor
}
