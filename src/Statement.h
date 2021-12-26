#ifndef SIMPLEDBMS_STATEMENT_H
#define SIMPLEDBMS_STATEMENT_H

#include <sstream>


namespace Interpreter {

    class Statement {
    public:
        Statement() = default;
        virtual ~Statement() = default;
        virtual void callAPI() = 0;
        virtual void printStatement() = 0;
    };

}

#endif //SIMPLEDBMS_STATEMENT_H
