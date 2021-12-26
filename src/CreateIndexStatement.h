#pragma once
#include "Statement.h"
#include "API.h"
#include "Exception.h"
#include "Property.h"
#include "Predicate.h"
#include "Table.h"
#include <sstream>

namespace Interpreter {
    //Creating an index on a field in a table creates another data structure which holds the field value, and a pointer
    // to the record it relates to. This index structure is
    // then sorted, allowing Binary Searches to be performed on it.

        class CreateIndexStatement : public Statement
        {
        public:
            void callAPI() override;
            void printStatement() override;
            void setIndexName(const std::string &);
            void setTableName(const std::string &);
            void setPropertyName(const std::string &);
        private:
            std::string indexName;
            std::string tableName;
            std::string propertyName;
        };


    }
