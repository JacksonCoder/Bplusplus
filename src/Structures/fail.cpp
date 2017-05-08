#include "fail.h"
void fail(std::string why)
{
    std::cout<<"An error has occured while compiling. This is the message\n============================="<<std::endl;
    std::cerr << why << std::endl;
    exit(1);
}
