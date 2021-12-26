#pragma once
#include <string>
#include "Value.h"
#include <vector>
#include <memory>
#include <cstring>
#include "Exception.h"

//shared_ptr - a smart pointer that shows shared ownership of an object through a pointer.
typedef std::vector<std::shared_ptr<std::vector<Value>>> View;

//Indexing is a way of sorting a number of records on multiple fields.
struct Index
{
    std::string indexName;
    std::string tableName;
    std::string propertyName;
    Index(std::string indexName, std::string tableName, std::string propertyName)
            : indexName(indexName), tableName(tableName), propertyName(propertyName) {}
};

