# B++
B++ is a language that compiles to C++ with the singular goal of being cleaner, simpler, but powerful.
In C++, you might have code that looks like this:
```cpp
#include <iostream>
#include <cmath>
#include <vector>
#include "math.hpp"
std::vector<int> results;
int main(const int argc,char** argv){
  std::cout<<"Input 12 numbers\n";
  for(int i = 0; i < 12; i++){
    std::cin>>results[i];
  }
  for(int i = 0; i < 12; i++){
    std::cout<<sqrt(results[i])<<std::endl;
  }
  return 0;
}
```
Messy, no?

That same thing in B++ would look much simpler:
```cpp
from math import sqrt;
global Container{int} results;

#(EXPORT)
main(const int argc, **char argv) -> int {
  @print("Input 12 numbers\n");
  for i = 1, i < 12, i++ {
    @input(results[i]);
  } then {
    @print(sqrt(results[i]));
  }
  0
}
```
Features of B++:
- Cleaner syntax
- Module system
- Smart pointers by default
- Type inference

A guide to the B++ language can be found inside `LANGUAGE.md`.

This compiler has the job of turning B++ code into C++ code. Note that this is *very much a work in progress*, and 

To get this thing working, you will need to first ```git clone``` the project (you should have git installed first):
``` 
git clone https://github.com/JacksonCoder/Bplusplus
```
To compile, use the Makefile:
```
make compiler
```
To remove object files for a clean build:
```
make clean
```

To run some tests, type:
```
make run_example
```

This project is a rennovation of a project I worked on a long time ago. This
project is less ambitious and more focused on clean design.
