#pragma once
#include <iostream>
#include "Statement.h"
#include "API.h"
#include "Exception.h"
#include "Property.h"
#include "Predicate.h"
#include "Table.h"
#include <sstream>

namespace Interpreter {
    class DeleteStatement : public Statement{
    public:
        void setTableName(const std::string &);
        void addPredicate(const Predicate &);
        void callAPI() override;
        void printStatement() override;

    private:
        std::string tableName;
        std::vector<Predicate> predicates;


    };

}