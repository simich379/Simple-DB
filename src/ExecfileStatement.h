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

    class ExecfileStatement : public Statement
    {
    public:
        void setFilePath(const std::string &);
        std::string getFilePath();
        void printStatement() override;
        void callAPI() override;
    private:
        std::string filePath;
    };

}