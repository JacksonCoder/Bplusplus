#ifndef BPPERROR_H
#define BPPERROR_H
#include "Libraries/shared.h"
class BPPError
{
    public:
        /** Default constructor */
        BPPError();
        BPPError(std::string);
        /** Default destructor */
        ~BPPError();
        /** Access message
         * \return The current value of message
         */
        std::string getMessage() { return message; }
        /** Set message
         * \param val New value to set
         */
        void setMessage(std::string val) { message = val; }
        /** Check for error
            \return Whether an error has occured
        */
        bool errorExists(){ return isError;}

        void setError(bool b){ isError = b;}
    private:
        std::string message; //!< Member variable "message"
        bool isError;
};

#endif // BPPERROR_H
