#include "../../Include/File.h"
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
            getline(stream,temp);
            contents+=temp+'\n';
    }
    std::cout<<contents<<std::endl;
}
