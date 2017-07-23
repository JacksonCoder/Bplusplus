#include "../../Include/File.h"
File::~File()
{
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
    stream.close();
}

void File::write(std::string filename,std::string output)
{
  stream.open(filename);
  if(!stream.is_open())
    stream.open(filename, std::ios::in | std::ios::out | std::ios::trunc);
  if(!stream.is_open()){ fail("Error on writing to file:" + filename);}
  stream<<output;
  debug("Wrote sucessfully to file " + filename);
  stream.close();
}
