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
    class QuitStatement : public Statement
    {
    public:
        void callAPI() override;
        void printStatement() override;
    };
}
