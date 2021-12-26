#pragma once
#include <string>
#include <memory>
#include <map>
#include "Exception.h"
#include "IndexManager.h"
#include "BufferManager.h"

#include "Type.h"
#include "Table.h"
#include "Index.h"

namespace CM {

    bool hasTable(std::string & tableName);
    std::shared_ptr<Table> findTable(std::string & tableName);
    void createTable(Table &table);
    void dropTable(Table &table);

    bool hasIndex(std::string indexName);
    Index* findIndexByName(std::string & indexName);
    Index* findIndexByTable(std::string & tableName, std::string & propertyName);
    void createIndex(Index& index);
    void dropIndex(Index& index);

    void updateIndexCache();
    void ensureIndexFile();


    //The map at file scope uses list initialization or an expression which may invoke a lambda.
    // In a class it's  wrapped in a static function to work around object initialization order
    // being undefined across multiple compilation units.
    static std::map<std::string, std::shared_ptr<Table>> tableCache;
    // indexName -> position in file
    static std::map<std::string, int> indexPositionCache;
    // indexName -> Index*
    static std::map<std::string, Index*> indexCache;
}