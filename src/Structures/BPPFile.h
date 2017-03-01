#ifndef BPPFILE_H
#define BPPFILE_H
#include "Libraries/shared.h"
#ifndef INCLUDEFAIL
#include "Compiler/fail.h"
#endif
class BPPFile
{
    public:
        /** Default constructor */
        BPPFile(){}
        /** Default destructor */
        ~BPPFile();
        std::string getContents() { return contents; }
        void read(std::string);
        std::vector<std::string> getLines(){ return lines;}
                std::vector<std::string> lines;
    protected:
    private:
        std::fstream stream; //!< Member variable "stream"

        std::string contents;
};

#endif // BPPFILE_H
