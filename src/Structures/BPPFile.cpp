#include "BPPFile.h"
#include "BPPError.h"
#include "Libraries/shared.h"

BPPFile::~BPPFile()
{
    stream.close();
}

void BPPFile::read(std::string filename)
{
    std::string templine;
    stream.open(filename);
    if(!stream.is_open()){  error.setMessage("Error on opening file... cannot open " + filename);}
    //stream file contents into local data;
    else while(!stream.eof()){
            getline(stream,templine);
    while(templine[templine.length()-1]=='|')
        {
            templine.erase(templine.end()-1,templine.end());
            if(stream.eof())
            {
                error.setError(true);
                error.setMessage("Error: Continous statement is terminated by EOF.");
                return;
            }
            std::string buffer;
            getline(stream,buffer);
            //if(buffer[buffer.length()-1]!='|') break;

            templine = templine + buffer;
        }
        contents += templine + '\n';
        lines.push_back(templine);
    }
}
