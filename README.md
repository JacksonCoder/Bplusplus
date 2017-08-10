# Please Read:
B++ is not longer being developed. It wasn't properly structured to begin with, and the vunerabilities and flaws of this thing are huge. I realized that development on such a system would eventually get so tedious I would have to just give up. I decided to discontinue this project, partially because I don't have as much time anymore (I'm being contracted by a tech company, and I have to work quite intensively), and also because I just wasn't happy with the direction this project was headed.

But I'm not nessecarily throwing away the project. I'm salvaging the pure algorithmic bits and parsing functions, which is what I spent the majority of my time on, and inject those into a new system, with more structure and design. It's a language called Synct, and trust me, it's going to be very similar to B++. You can check out the new compiler at https://github.com/JacksonCoder/Synct.

I'll leave the installation instructions and introduction on here, just in case people want to test it out. Just to warn you, it has numerous bugs.

Be sure to check out my new compiler, and have a good day!

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
Container{int}results
main(const int argc,pointer pointer char argv)
  out("Input 12 numbers\n")
  for int i : range(0,12)
    in(results[i])
  for int i : range(0,12)
    out(sqrt(results[i))
  return 0
```
Features of B++:
- Built-in asyncronous function support
- Memory-safe
- Cleaner syntax
- Inline Python scripting
- Use native C++ functions and classes
- Type inference
- Minimal overhead

This compiler has the job of turning B++ code into C++ code. Some error-checking is handled, but not all of it. We leave it up to the C++ compiler's much more robust error-checker to find things like template errors and such.

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

This project is still a work in progress, but feel free to use it.
