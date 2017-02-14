#ifndef BPPFILE_H
#define BPPFILE_H
#include "Libraries/shared.h"
#include "Structures/BPPError.h"
class BPPFile
{
    public:
        /** Default constructor */
        BPPFile(){}
        /** Default destructor */
        ~BPPFile();
        std::string getContents() { return contents; }

        bool errorOccured() {return error.errorExists();}
        void read(std::string);
        std::vector<std::string> getLines(){ return lines;}
                std::vector<std::string> lines;
    protected:
    private:
        std::fstream stream; //!< Member variable "stream"

        std::string contents;
        BPPError error;
};

#endif // BPPFILE_H
