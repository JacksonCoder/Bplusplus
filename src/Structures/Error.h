#ifndef ERROR_H
#define ERROR_H
#include "../Libraries/shared.h"
class Error
{
    public:
        /** Default constructor */
        Error();
        Error(std::string);
        /** Default destructor */
        ~Error();
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

#endif // ERROR_H
