#ifndef SHARED_H_INCLUDED
#define SHARED_H_INCLUDED
#include "../../Include/tokenType.h"
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <algorithm>
#include <cassert>
#include <regex>
extern bool isdebug;
inline void debug(std::string string){
  if(isdebug)
    std::cout<<"Debug output:" + string<<std::endl;
}
#endif // SHARED_H_INCLUDED
