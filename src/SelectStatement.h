#pragma once
#include <iostream>
#include "Statement.h"
#include "API.h"
#include "Exception.h"
#include "Property.h"
#include "Predicate.h"
#include "Table.h"
#include <sstream>

namespace Interpreter{
    class SelectStatement : public Statement{
    public:
        void addRequiredProperty(const std::string &);
        void addPredicate(const Predicate &);
        void setTableName(const std::string &);
        void callAPI() override;
        void printStatement() override;
        template<typename T>
        inline int getLength(const T& t) const {
            std::ostringstream os;
            os << t;
            return os.str().size();
        }

    private:
        std::vector<std::string> properties;
        std::string tableName;
        std::vector<Predicate> predicates;
    };
}