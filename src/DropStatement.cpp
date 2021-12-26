#include <iostream>
#include "DropStatement.h"
namespace Interpreter {
    void DropStatement::setTableName(const std::string &s) {
        tableName = s;
    }

    void DropStatement::callAPI() {
        API::SQLResult<void> res = API::dropTable(tableName);
        if (res.isSuccess) {
            std::cout << "Table \'" << tableName
                      << "\' dropped successfully in " << res.durationMS
                      << " ms" << std::endl;
        } else {
            std::cout << "Drop Table fails: " + res.errorMessage << std::endl;
        }
    }

    void DropStatement::printStatement() {
        std::cout << "DROP TABLE" << std::endl;
        std::cout << "\t " << tableName << std::endl;
    }
}