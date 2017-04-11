# B++
What is B++? B++ is a language with an end goal to be more simple, straightforward, and less irritating than C++, while at the same time, retaining the power of C++.
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
That same thing in B++ would look much simpler, and is easier to read:
```cpp
import native local math.hpp,native local vector, sdk
extern std/vector
results:std/vector<int>
main(const argc:int,argv:char**)
  stream "Input 12 numbers\n" to std/cout
  repeat 12 iterator:int
    stream vector[iterator] from std/cin
  end
  repeat 12 iterator:int
    stream sqrt(vector[iterator]),std/endl to std/cout
  end
  <- 0
end
```
This compiler has the job of turning B++ code into C++ code. Some error-checking is handled, but not all of it. We leave it up to the C++ compiler's much more robust error-checker to find things like template errors and such.

To get this thing working, you will need to first ```git clone``` the project (you should have git installed first):
``` git clone https://github.com/JacksonCoder/Bplusplus```
to compile, use the Makefile:
'''make all'''
to run some tests, type:
```make runtest```

Enjoy!
