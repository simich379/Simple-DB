#ifndef SIMPLEDBMS_PARSERERROR_H
#define SIMPLEDBMS_PARSERERROR_H

#include <stdexcept>
#include <string>

namespace Interpreter {

    class ParserError : public std::runtime_error {
    public:
        explicit ParserError(const std::string &msg, int ln, int cn);
    };

}

#endif //SIMPLEDBMS_PARSERERROR_H
