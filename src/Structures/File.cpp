#include "File.h"
File::~File()
{
    stream.close();
}

void File::read(std::string filename)
{
    std::string templine;
    stream.open(filename);
    if(!stream.is_open()){  fail("Error on opening file... cannot open " + filename); }
    //stream file contents into local data;
    else while(!stream.eof()){
            stream >> contents;
    }
}
