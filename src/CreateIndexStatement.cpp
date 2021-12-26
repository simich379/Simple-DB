#include "CreateIndexStatement.h"
#include <iostream>

namespace Interpreter {
    void CreateIndexStatement::callAPI() {
        Index index(indexName, tableName, propertyName);
        API::SQLResult<void> res = API::createIndex(index);
        if (res.isSuccess) {
            std::cout << "Index \'" << indexName
                      << "\' created on property \'" << propertyName
                      << "\' successfully in " << res.durationMS
                      << " ms" << std::endl;
        } else {
            std::cout << "Create Index fails: " + res.errorMessage << std::endl;
        }
    }

    void CreateIndexStatement::printStatement() {
        std::cout << "CREATE INDEX" << std::endl;
        std::cout << "\t " << indexName << std::endl;
        std::cout << "\t " << tableName << std::endl;
        std::cout << "\t " << propertyName << std::endl;
    }

    void CreateIndexStatement::setIndexName(const std::string &s) {
        indexName = s;
    }

    void CreateIndexStatement::setTableName(const std::string &s) {
        tableName = s;
    }

    void CreateIndexStatement::setPropertyName(const std::string &s) {
        propertyName = s;
    }
}