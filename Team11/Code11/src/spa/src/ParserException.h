#include <exception>
#include <string>

class ParserException : public std::exception {

protected:

    std::string errorMessage;
public:
    ParserException(const std::string parseExceptionMessage) : errorMessage(parseExceptionMessage) {}

    ~ParserException() throw () {}

    const char* what() const throw ()
    {
        return errorMessage.c_str();
    }
};