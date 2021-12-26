#include "InsertStatement.h"

namespace Interpreter{
    void InsertStatement::setTableName(const std::string & s)
    {
        tableName = s;
    }

    void InsertStatement::addValue(const Value & v)
    {
        values.push_back(v);
    }

    void InsertStatement::callAPI()
    {
        API::SQLResult<void> res = API::insert(tableName, values);
        if (res.isSuccess) {
            std::cout << "Insertion on Table \'" << tableName
                      << "\' is successful in " << res.durationMS
                      << " ms" << std::endl;
        }
        else {
            std::cout << "Insert Value fails: " + res.errorMessage << std::endl;
        }
    }

    void InsertStatement::printStatement()
    {
        std::cout << "INSERT" << std::endl;

        for (auto value : values) {
            switch (value.getType().getBaseType())
            {
                case BaseType::BOOL:
                    std::cout << "\t "
                              << *(value.getAsType<bool>())
                              << std::endl;
                    break;
                case BaseType::INT:
                    std::cout << "\t "
                              << *(value.getAsType<int>())
                              << std::endl;
                    break;
                case BaseType::FLOAT:
                    std::cout << "\t "
                              << *(value.getAsType<double>())
                              << std::endl;
                    break;
                case BaseType::VARCHAR:
                    std::cout << "\t "
                              << value.getAsType<char>()
                              << std::endl;
                    break;
                default:
                    break;
            }
        }
    }
}