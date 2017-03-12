#include "File.h"
File::~File()
{
    stream.close();
}

void File::read(std::string filename)
{
    stream.open(filename);
    if(!stream.is_open()){  fail("Error on opening file... cannot open " + filename);}
    //stream file contents into local data;
    while(!stream.eof()){
            std::string temp;
            stream >> temp;
            contents+=temp;
            if(stream.peek()=='\n') contents+='\n';
    }
    std::cout<<contents<<std::endl;
}
