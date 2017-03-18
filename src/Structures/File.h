#ifndef FILE_H
#define FILE_H
#include "../Libraries/shared.h"
#include "../Compiler/fail.h"
class File
{
    public:
        /** Default constructor */
        File(){}
        /** Default destructor */
        ~File();
        std::string getContents() { return contents; }
        void read(std::string);
    protected:
    private:
        std::fstream stream; //!< Member variable "stream"

        std::string contents;
};

#endif // FILE_H
