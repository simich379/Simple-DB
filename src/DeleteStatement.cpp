#include <iomanip>
#include "DeleteStatement.h"

namespace Interpreter{
    void DeleteStatement::setTableName(const std::string & s)
    {
        tableName = s;
    }

    void DeleteStatement::addPredicate(const Predicate & p)
    {
        predicates.push_back(p);
    }

    void DeleteStatement::callAPI()
    {
        API::SQLResult<int> res = API::deleteFrom(tableName, predicates);
        if (res.isSuccess) {
            std::cout << *(res.result)
                      << " rows deleted successful in " << res.durationMS
                      << " ms" << std::endl;
        }
        else {
            std::cout << "Delete Value fails: " + res.errorMessage << std::endl;
        }
    }

    void DeleteStatement::printStatement()
    {
        std::cout << "DELETE" << std::endl;
        std::cout << "\t " << tableName << std::endl;
        for (auto predicate : predicates) {
            std::cout << "\t "
                      << std::left
                      << std::setw(31)
                      << predicate.propertyName
                      << std::setw(31)
                      << predicate.op;
            switch (predicate.val.getType().getBaseType())
            {
                case BaseType::BOOL:
                    std::cout << std::left
                              << std::setw(31)
                              << *(predicate.val.getAsType<bool>())
                              << std::endl;
                    break;
                case BaseType::INT:
                    std::cout << std::left
                              << std::setw(31)
                              << *(predicate.val.getAsType<int>())
                              << std::endl;
                    break;
                case BaseType::FLOAT:
                    std::cout << std::left
                              << std::setw(31)
                              << *(predicate.val.getAsType<double>())
                              << std::endl;
                    break;
                case BaseType::VARCHAR:
                    std::cout << std::left
                              << std::setw(31)
                              << predicate.val.getAsType<char>()
                              << std::endl;
                    break;
                default:
                    break;
            }
        }
    }
}