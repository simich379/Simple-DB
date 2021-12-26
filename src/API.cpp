
#include "API.h"
#include "CatalogManager.h"
#include "RecordManager.h"
#define getPrimaryIndexName(tableName, primaryKey) (tableName + "_" + primaryKey + "_primary")
namespace API {

    SQLResult<void> createTable(Table &table) {
        clock_t start = clock();
        try {
            CM::createTable(table);
            RM::createTable(table);
            Index *index = new Index(
                    getPrimaryIndexName(table.tableName, table.primaryKey),
                    table.tableName, table.primaryKey
            );
            auto indexCre = createIndex(*index);
            bufferManager.globalSynchronize();
            if (!indexCre.isSuccess) {
                return SQLResult<void>(nullptr, clock() - start, false, indexCre.errorMessage);
            }
            return SQLResult<void>(nullptr, clock() - start, true, "");
        }
        catch (SQLException e) {
            return SQLResult<void>(nullptr, clock() - start, false, e.msg);
        }
    }
    SQLResult<void> dropTable(Table &table) {
        clock_t start = clock();
        try {
            CM::dropTable(table);
            RM::dropTable(table);
            bufferManager.globalSynchronize();
            return SQLResult<void>(nullptr, clock() - start, true, "");
        }
        catch (SQLException e) {
            return SQLResult<void>(nullptr, clock() - start, false, e.msg);
        }
    }

    SQLResult<void> createIndex(Index &index) {
        clock_t start = clock();
        try {
            // check property valid
            auto tablePtr = CM::findTable(index.tableName);
            int i = 0;
            for (i = 0; i < tablePtr->properties.size(); i++) {
                if (tablePtr->properties[i].name == index.propertyName) {
                    break;
                }
            }
            if (i == tablePtr->properties.size()) {
                throw SQLException("Property not found: " + index.propertyName);
            }
            CM::createIndex(index);
            IndexManager::createNewIndex(index);
            // insert old values
            IndexManager indexManager(index);
            size_t recordLen = RM::getRecordSize(*tablePtr);
            int recordsPerBlock = BLOCKSIZE / recordLen;
            RM::forEachLinear(*tablePtr, *(new std::vector<Predicate>),
                              [&](BlockNode* bn, size_t offset, std::shared_ptr<std::vector<Value>> record) {
                                  int id = bn->offset * recordsPerBlock + offset / recordLen;
                                  indexManager.insertEntry(&record->at(i), id);
                                  return true; // go on
                              });
            bufferManager.globalSynchronize();
            return SQLResult<void>(nullptr, clock() - start, true, "");
        }
        catch (SQLException e) {
            return SQLResult<void>(nullptr, clock() - start, false, e.msg);
        }
    }

    SQLResult<void> dropIndex(std::string indexName) {
        clock_t start = clock();
        try {
            Index* index = CM::findIndexByName(indexName);
            CM::dropIndex(*index);
            IndexManager::dropIndex(*index);
            bufferManager.globalSynchronize();
            return SQLResult<void>(nullptr, clock() - start, true, "");
        }
        catch (SQLException e) {
            return SQLResult<void>(nullptr, clock() - start, false, e.msg);
        }
    }

    SQLResult<std::pair<View, std::vector<std::string>>> select(
            std::vector<std::string> &properties,
            std::string &tableName,
            std::vector<Predicate> &predicates
    ) {
        clock_t start = clock();
        try {
            auto tablePtr = CM::findTable(tableName);
            View result;
            // reset char's size
            for (int i = 0; i < predicates.size(); i++) {
                if (predicates[i].val.getType() == BaseType::VARCHAR) {
                    for (int j = 0; j < tablePtr->properties.size(); j++) {
                        if (tablePtr->properties[j].name == predicates[i].propertyName) {
                            Type type = predicates[i].val.getType();
                            type.setType(BaseType::VARCHAR, tablePtr->properties[j].type.getSize());
                            predicates[i].val.setType(type);
                            break;
                        }
                    }
                }
            }
            std::vector<string> propertyStrings;
            if (properties.size() != 0) {
                propertyStrings = properties;
                result = RM::selectRecords(properties, *tablePtr, predicates);
            } else {
                for (int i = 0; i < tablePtr->properties.size(); i++) {
                    propertyStrings.push_back(tablePtr->properties[i].name);
                }
                result = RM::selectRecords(propertyStrings, *tablePtr, predicates);
            }
            return SQLResult<std::pair<View, std::vector<std::string>>>(
                    std::make_shared<std::pair<View, std::vector<std::string>>>
                            (std::make_pair(result, propertyStrings)), clock() - start, true, std::string(""));
        }
        catch (SQLException e) {
            return SQLResult<std::pair<View, std::vector<std::string>>>(
                    nullptr, clock() - start, true, e.msg);
        }
    }

    SQLResult<int> deleteFrom(
            std::string &tableName,
            std::vector<Predicate> &predicates
    ) {
        clock_t start = clock();
        try {
            int result = 0;
            auto tablePtr = CM::findTable(tableName);
            result = RM::deleteRecords(*tablePtr, predicates);
            bufferManager.globalSynchronize();
            return SQLResult<int>(
                    std::make_shared<int>(result), clock() - start, true, "");
        }
        catch (SQLException e) {
            return SQLResult<int>(
                    std::make_shared<int>(), clock() - start, false, e.msg);
        }
    }

    SQLResult<void> dropTable(std::string & tableName) {
        clock_t start = clock();
        try {
            auto table = CM::findTable(tableName);
            CM::dropTable(*table);
            RM::dropTable(*table);
            // drop all indexes of table
            for (int i = 0; i < table->properties.size(); i++) {
                string propertyName(table->properties[i].name);
                auto index = CM::findIndexByTable(tableName, propertyName);
                if (index != nullptr) {
                    CM::dropIndex(*index);
                    IndexManager::dropIndex(*index);
                }
            }
            bufferManager.globalSynchronize();
            return SQLResult<void>(
                    nullptr, clock() - start, true, "");
        }
        catch (SQLException e) {
            return SQLResult<void>(
                    nullptr, clock() - start, false, e.msg);
        }
    }

    SQLResult<void> insert(
            std::string &tableName,
            std::vector<Value> &values
    ) {
        clock_t start = clock();
        try {
            auto table = CM::findTable(tableName);
            // reset char's size
            auto properties = table->properties;
            for (int i = 0; i < properties.size(); i++) {
                if (properties[i].type.getBaseType() == BaseType::VARCHAR) {
                    Type type = values[i].getType();
                    type.setType(BaseType::VARCHAR, properties[i].type.getSize());
                    values[i].setType(type);
                }
            }
            RM::insertRecord(*table, values);
            bufferManager.globalSynchronize();
            return SQLResult<void>(
                    nullptr, clock() - start, true, "");
        }
        catch (SQLException e) {
            return SQLResult<void>(
                    nullptr, clock() - start, false, e.msg);
        }
    }
}