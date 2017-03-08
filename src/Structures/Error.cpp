#include "Error.h"

Error::Error()
{
    isError = false;
}
Error::Error(std::string cons) : message(cons){
    isError = true;
}
Error::~Error()
{
    //dtor
}
