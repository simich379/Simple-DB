#include <iomanip>
#include "SelectStatement.h"

namespace Interpreter{
    void DrawSymbol(int n, char c) {
        for (int i = 0; i < n; i++) {
            std::cout << c;
        }
    }

    void SelectStatement::addRequiredProperty(const std::string & s)
    {
        properties.push_back(s);
    }

    void SelectStatement::addPredicate(const Predicate & p)
    {
        predicates.push_back(p);
    }

    void SelectStatement::setTableName(const std::string & s)
    {
        tableName = s;
    }

    void SelectStatement::callAPI()
    {
        //a class template that provides a way to store two heterogeneous objects as a single unit;
        // pair ia a specific case of std::tuple with 2 elements
        API::SQLResult<std::pair<View, std::vector<std::string>>> res = API::select(properties, tableName, predicates);
        if (res.isSuccess) {
            View v = res.result->first;// take the first object
            int tupleCnt = v.size();
            if (tupleCnt == 0) {
                std::cout << "No record selected." << std::endl;
                return;
            }

            std::vector<std::string> properties = res.result->second;
            int attrCnt = properties.size();

            // calculate the maxWidth of each column
            std::vector<int> maxWidth(attrCnt, 0);
            for (int i = 0; i < attrCnt; i++) {
                maxWidth[i] = std::max(maxWidth[i], (int)properties[i].size());
            }

            for (int i = 0; i < tupleCnt; i++) {
                for (int j = 0; j < attrCnt; j++) {
                    Value *val = &(v[i]->at(j)); //Access the  element
                    Type type = val->getType();

                    switch (type.getBaseType())
                    {
                        case BaseType::BOOL:
                            maxWidth[j] = std::max(maxWidth[j], getLength(*(val->getAsType<bool>())));
                            break;
                        case BaseType::INT:
                            maxWidth[j] = std::max(maxWidth[j], getLength(*(val->getAsType<int>())));
                            break;
                        case BaseType::FLOAT:
                            maxWidth[j] = std::max(maxWidth[j], getLength(*(val->getAsType<double>())));
                            break;
                        case BaseType::VARCHAR:
                            maxWidth[j] = std::max(maxWidth[j], getLength(val->getAsType<char>()));
                            break;
                        default:
                            break;
                    }
                }
            }

            // Draw output table header
            for (int i = 0; i < attrCnt; i++) {
                std::cout << "+";
                DrawSymbol(maxWidth[i] + 2, '-');
            }
            std::cout << "+";
            std::cout << std::endl;

            for (int i = 0; i < attrCnt; i++) {
                std::cout << "|"
                          << std::internal
                          << std::setw(maxWidth[i] + 2)
                          << properties[i];
            }
            std::cout << "|" << std::endl;

            for (int i = 0; i < attrCnt; i++) {
                std::cout << "+";
                DrawSymbol(maxWidth[i] + 2, '-');
            }
            std::cout << "+";
            std::cout << std::endl;

            // draw output table content
            for (int i = 0; i < tupleCnt; i++) {
                for (int j = 0; j < attrCnt; j++) {
                    Value *val = &((*v[i])[j]);
                    Type t = (val->getType());
                    switch (t.getBaseType())
                    {
                        case BaseType::BOOL:
                            std::cout << "|"
                                      << std::setw(maxWidth[j] + 2)
                                      << *(val->getAsType<bool>());
                        case BaseType::INT:
                            std::cout << "|"
                                      << std::setw(maxWidth[j] + 2)
                                      << *(val->getAsType<int>());
                            break;
                        case BaseType::FLOAT:
                            std::cout << "|"
                                      << std::setw(maxWidth[j] + 2)
                                      << *(val->getAsType<double>());
                            break;
                        case BaseType::VARCHAR:
                            std::cout << "|"
                                      << std::setw(maxWidth[j] + 2)
                                      << val->getAsType<char>();
                            break;
                        default:
                            break;
                    }
                }
                std::cout << "|" << std::endl;
            }

            for (int i = 0; i < attrCnt; i++) {
                std::cout << "+";
                DrawSymbol(maxWidth[i] + 2, '-');
            }
            std::cout << "+";
            std::cout << std::endl;

            std::cout << tupleCnt << " rows in set (" << res.durationMS << " ms)" << std::endl;
        }
        else {
            std::cout << "Select fails: " + res.errorMessage << std::endl;
        }
    }

    void SelectStatement::printStatement()
    {
        std::cout << "SELECT" << std::endl;
        for (auto property : properties) {
            std::cout << "\t "
                      << property
                      << std::endl;
        }
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