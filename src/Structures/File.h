#ifndef FILE_H
#define FILE_H
#include "Libraries/shared.h"
#ifndef INCLUDEFAIL
#include "Compiler/fail.h"
#endif
class File
{
    public:
        /** Default constructor */
        File(){}
        /** Default destructor */
        ~File();
        std::string getContents() { return contents; }
        void read(std::string);
        std::vector<std::string> getLines(){ return lines;}
                std::vector<std::string> lines;
    protected:
    private:
        std::fstream stream; //!< Member variable "stream"

        std::string contents;
};

#endif // FILE_H
