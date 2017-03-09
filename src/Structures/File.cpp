#include "File.h"
File::~File()
{
    stream.close();
}

void File::read(std::string filename)
{
    std::string temp;
    stream.open(filename);
    if(!stream.is_open()){  fail("Error on opening file... cannot open " + filename);}
    //stream file contents into local data;
    while(!stream.eof()){
            stream >> temp;
            if(stream.peek()=='\n') temp += "\n";
            contents += temp;
    }
    std::cout<<contents<<std::endl;
}
