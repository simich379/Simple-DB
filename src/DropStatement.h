#ifndef SIMPLEDBMS_DROPSTATEMENT_H
#define SIMPLEDBMS_DROPSTATEMENT_H

#include "Statement.h"
#include "API.h"
#include "Exception.h"
#include "Table.h"
#include <sstream>


namespace Interpreter {
    class DropStatement : public Statement {
    public:
        void callAPI() override;
        void printStatement() override;
        void setTableName(const std::string &);

    private:
        std::string tableName;

    };
}


#endif //SIMPLEDBMS_DROPSTATEMENT_H
