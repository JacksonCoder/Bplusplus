#include "BPPError.h"

BPPError::BPPError()
{
    isError = false;
}
BPPError::BPPError(std::string cons) : message(cons){
    isError = true;
}
BPPError::~BPPError()
{
    //dtor
}
