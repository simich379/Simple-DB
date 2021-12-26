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
    class DropIndexStatement : public Statement
    {
    public:
        void setIndexName(const std::string &);
        void callAPI() override;
        void printStatement() override;
    private:
        std::string indexName;
    };
}


