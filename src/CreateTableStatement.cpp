#include "Exception.h"
#include "CreateTableStatement.h"
#include <iostream>
#include <iomanip>
#include <algorithm>



namespace Interpreter {

    void CreateTableStatement::callAPI() {
        Table table(tableName, primaryKey, properties);

        API::SQLResult<void> res = API::createTable(table);
        if (res.isSuccess) {
            std::cout << "Table \'" << tableName
                      << "\' created successfully in " << res.durationMS
                      << " ms" << std::endl;
        } else {
            std::cout << "Create Table fails: " + res.errorMessage << std::endl;
        }
    }

    void CreateTableStatement::printStatement() {
        std::cout << "CREATE TABLE" << std::endl;
        std::cout << "\t " << tableName << std::endl;
        std::cout << "\t " << primaryKey << std::endl;
        for (auto property : properties) {
            std::cout << "\t "
                      << property.name
                      << "\t ";
            if (property.unique)
                std::cout << "Unique" << std::endl;
            else std::cout << std::endl;
        }

    }

    std::basic_string<char> CreateTableStatement::setTableName(const std::string &s) {
        tableName = s;
        return tableName;
    }

    void CreateTableStatement::setPrimaryKey(std::string &s) {
        primaryKey = s;
    }

    void CreateTableStatement::addProperty(const Property &p) {
        properties.push_back(p);
    }
}