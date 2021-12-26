#include <iostream>
#include "API.h"
#include "BufferManager.h"
#include "RecordManager.h"
#include "CatalogManager.h"
#include "Exception.h"

#define RECORD_FILE(x) (x + ".db")

namespace RM {

    void freeRecord(std::shared_ptr<std::vector<Value>> record) {
        for (int i = 0; i < record->size(); i++) {
            switch (record->at(i).getType().getBaseType()) {
                case BaseType::INT:
                    delete record->at(i).getAsType<int>();
                    break;
                case BaseType::BOOL:
                    delete record->at(i).getAsType<bool>();
                    break;
                case BaseType::FLOAT:
                    delete record->at(i).getAsType<double>();
                    break;
                case BaseType::VARCHAR:
                    delete[] record->at(i).getAsType<char>();
                    break;
            }
        }
    }

    bool hasTable(std::string & tableName) {
        return bufferManager.JudgeFileExistence(RECORD_FILE(tableName));   //judge the struct if exists
    }

    void createTable(Table &table) {
        if (hasTable(table.tableName)) {
            throw SQLException("table " + table.tableName + "exists");      //throw the file exist exception
        }
        else {
            //CM::createTable(table);
            bufferManager.CreateFile(RECORD_FILE(table.tableName));
        }
    }

    void dropTable(Table& table){
        if (hasTable(table.tableName)) {
            bufferManager.DeleteFile(RECORD_FILE(table.tableName));
        }
        else {
            throw SQLException("Table doesn't exist!");
        }
    }

    void insertRecord(Table& table, std::vector<Value> record) {
        checkExist(table);
        checkTypes(table, record);
        // check unique
        for (int i = 0; i < table.properties.size(); i++) {
            if (table.properties[i].unique) {
                bool uniqueCheck = true;
                // Find the record
                std::string propertyName(table.properties[i].name);
                Predicate predicate(propertyName, OpType::EQ, record[i]);
                std::vector<Predicate> predicates;
                predicates.push_back(predicate);
                forEach(table, predicates, [&](BlockNode* bn, size_t offset, std::shared_ptr<std::vector<Value>> record) {
                    // find!
                    uniqueCheck = false;
                    return false; // no need to search more
                });
                if (!uniqueCheck) {
                    throw SQLException("Unique check failed: " + propertyName);
                }
            }
        }
        size_t recordLen = getRecordSize(table);
        int recordsPerBlock = BLOCKSIZE / recordLen;

        FileNode* file = bufferManager.GetFile(RECORD_FILE(table.tableName));
        int blockNum = file->getBlockNum();
        if (blockNum == 0) {
            BlockNode* block = new BlockNode;
            block->Data = new char[BLOCKSIZE];
            memset(block->Data, 0, BLOCKSIZE);
            file->allocNewNode(block);
            // write in the first
            writeRecord(record, block->Data);
            saveIndexes(table, record, block->offset * recordsPerBlock);

            block->dirty = true;
            return;
        } else {
            // Check if the last block has vacancy
            BlockNode* last = file->getblock(blockNum - 1);
            int i = 0;
            for (int i = 0; i < recordsPerBlock; i++) {
                int offset = i * recordLen;
                auto re = readRecord(table, last->Data + offset);
                if (re == nullptr) {
                    // insert here!
                    last->dirty = true;
                    writeRecord(record, last->Data + offset);
                    saveIndexes(table, record, last->offset * recordsPerBlock + i);
                    return;
                }
                else {
                    freeRecord(re);
                }
            }
        }
        // we must alloc a new block
        BlockNode* block = new BlockNode;
        block->Data = new char[BLOCKSIZE];
        memset(block->Data, 0, BLOCKSIZE);
        file->allocNewNode(block);
        block->dirty = true;
        // write in the first
        writeRecord(record, block->Data);
        saveIndexes(table, record, block->offset * recordsPerBlock);
    }

    static bool satisfy(string name, Predicate &predicate, Value &record) {
        if (predicate.propertyName == name) {
            auto &value = record;
            switch (predicate.op) {
                case OpType::EQ:
                    return value == predicate.val;
                case OpType::NE:
                    return value != predicate.val;
                case OpType::LT:
                    return value < predicate.val;
                case OpType::LEQ:
                    return value <= predicate.val;
                case OpType::GT:
                    return value > predicate.val;
                case OpType::GEQ:
                    return value >= predicate.val;
            }
        }
        throw SQLException("cannot find attribute \'" + predicate.propertyName +
                           "\' in table \'" + name + "\'");
    }

    int deleteAllRecords(Table& table){
        checkExist(table);
        int c = count(table);
        bufferManager.DeleteFile(RECORD_FILE(table.tableName));
        bufferManager.CreateFile(RECORD_FILE(table.tableName));
        return c;
    }

    int deleteRecords(
            Table& table,
            std::vector<Predicate> & predicates
    ){
        int count = 0;
        checkExist(table);
        forEach(table, predicates, [&](BlockNode* bn, size_t offset, std::shared_ptr<std::vector<Value>> record) {
            // find! mark unvalid!
            bool valid = false;
            memcpy(bn->Data + offset, &valid, sizeof(bool));
            bn->dirty = true;
            count++;
            //deleteIndexes(table, record);
            return true; // go on searching
        });

        return count;
    }

    View selectRecords(
            std::vector<std::string> &properties,
            Table& table,
            std::vector<Predicate> & predicates
    ) {
        View* view = new View;
        checkExist(table);
        forEach(table, predicates, [&](BlockNode* bn, size_t offset, std::shared_ptr<std::vector<Value>> record) {
            // find! add to view!
            auto projection = std::make_shared<std::vector<Value>>();
            for (int i = 0; i < properties.size(); i++) {
                for (int j = 0; j < table.properties.size(); j++) {
                    if (table.properties[j].name == properties[i]) {
                        Value* value = new Value(record->at(j), 1);

                        projection->push_back(*value);
                        break;
                    }
                }
            }
            view->push_back(projection);
            return true; // go on searching
        });
        return *view;
    }

    size_t getRecordSize(Table& table) {
        size_t totalSize = sizeof(bool);
        for (int i = 0; i < table.properties.size(); i++) {
            totalSize += table.properties[i].type.getSize();
        }
        return totalSize;
    }

    void checkExist(Table& table) {
        if (!hasTable(table.tableName)) {
            throw SQLException("table " + table.tableName + "doesn't exist");
        }
    }

    void checkTypes(Table& table, std::vector<Value> record) {
        for (int i = 0; i < record.size(); i++) {
            if (record[i].getType() != table.properties[i].type) {
                throw SQLException("Type doesn't match!");
            }
        }
    }

    // return null if the record is unvalid.
    std::shared_ptr<std::vector<Value>> readRecord(Table& table, char* ptr) {
        int p = 0;
        // First, read the valid flag;
        bool valid;
        memcpy(&valid, ptr + p, sizeof(bool));
        if (!valid) {
            return nullptr;
        }
        p += sizeof(bool);
        // Second, read records in order.
        auto records = std::make_shared<std::vector<Value>>();
        for (int i = 0; i < table.properties.size(); i++) {
            size_t size = table.properties[i].type.getSize();
            char* content = new char[size];
            memcpy(content, ptr + p, size);
            records->emplace_back();
            records->operator[](records->size() - 1).setType(reinterpret_cast<Type &>(content));
            records->operator[](records->size() - 1).setType(table.properties[i].type);
            p += size;
        }
        return std::shared_ptr<std::vector<Value>>(records);
    }

    void saveIndexes(Table& table, std::vector<Value> record, int id) {
        for (int i = 0; i < table.properties.size(); i++) {
            string propertyname(table.properties[i].name);
            Index* index = CM::findIndexByTable(table.tableName, propertyname);
            if (index != nullptr) {
                IndexManager im(*index);
                im.insertEntry(&record[i], id);
            }
        }
    }

    void deleteIndexes(Table& table, std::shared_ptr<std::vector<Value>> record) {
        for (int i = 0; i < table.properties.size(); i++) {
            string propertyname(table.properties[i].name);
            Index* index = CM::findIndexByTable(table.tableName, propertyname);
            if (index != nullptr) {
                IndexManager im(*index);
                im.deleteEntry(&record->at(i));
            }
        }
    }

    void writeRecord(std::vector<Value> records, char* ptr) {
        int p = 0;
        // First, write the valid flag
        bool valid = true;
        memcpy(ptr + p, &valid, sizeof(bool));
        p += sizeof(bool);
        // Second, insert records in order.
        for (int i = 0; i < records.size(); i++) {
            // save raw data instead Value struct
            memcpy(ptr + p, records[i].getAsType<char>(), records[i].getType().getSize());
            p += records[i].getType().getSize();
        }
    }

    int count(Table& table) {
        checkExist(table);
        int count = 0;
//        forEach(table, std::vector<Predicate>(),(BlockNode* bn, size_t offset, std::shared_ptr<std::vector<Value>> record) {
            // find! count++!
//            count++;
//            return true;
//        });
        return count;
    }

    bool checkPredicate(Table& table, vector<Predicate> predicates, const shared_ptr<vector<Value>>& re) {
        bool satis = true;
        for (int k = 0; k < predicates.size() && satis; k++) {
            Predicate p = predicates[k];
            for (int l = 0; l < table.properties.size() && satis; l++) {
                if (table.properties[l].name == p.propertyName) {
                    satis &= satisfy(table.properties[l].name, p, re->at(l));
                }
            }
        }
        return satis;
    }

    void forEach(
            Table& table,
            std::vector<Predicate>& predicates,
            const std::function<bool(BlockNode*, size_t, std::shared_ptr<std::vector<Value>>)>& consumer // (blocknode, offset, record) => (goOn)
    ) {
        Index* index = nullptr;
        int indexPos;
        for (int i = 0; i < predicates.size(); i++) {
            auto pre = predicates[i];
            index = CM::findIndexByTable(table.tableName, pre.propertyName);
            if (index != nullptr) {
                indexPos = i;
                break;
            }
        }
        if (index != nullptr) {
            // use index
            IndexManager indexManager(*index);
            OpType op = predicates[indexPos].op;
            Value indexValue = predicates[indexPos].val;
            FileNode* file = bufferManager.GetFile(RECORD_FILE(table.tableName));

            switch (op) {
                case NE: // <>
                    forEachLinear(table, predicates, consumer);
                    break;
                case LT:  // <
                case LEQ: {// <=
                    // begin from the left leave to find
                    auto iter = indexManager.findMinOrMax(true);
                    while (iter != nullptr) {
                        if (op == LT && *(iter->currentValue) >= indexValue) break;
                        if (op == LEQ && *(iter->currentValue) > indexValue) break;
                        forEachIndexLeave(table, iter, file, predicates, consumer);
                        iter = iter->next();
                    }
                    break;
                }
                case GT: // >
                case GEQ: // >=
                case EQ: // =
                    // use index to find the first.
                    auto iter = indexManager.find(&indexValue);
                    while (iter != nullptr) {
                        if (op == EQ && *iter->currentValue != indexValue) {
                            break;
                        }
                        forEachIndexLeave(table, iter, file, predicates, consumer);
                        iter = iter->next();
                    }
                    break;
            }
        }
        else {
            forEachLinear(table, predicates, consumer);
        }
    }

    void forEachIndexLeave(
            Table& table,
            const std::shared_ptr<IndexIterator>& iter,
            FileNode* file,
            std::vector<Predicate>& predicates,
            const std::function<bool(BlockNode*, size_t, std::shared_ptr<std::vector<Value>>)>& consumer // (blocknode, offset, record) => (goOn)
    ) {
        int id = iter->indexInRecord;
        int recordLen = getRecordSize(table);
        int recordsPerBlock = BLOCKSIZE / recordLen;
        int blockIndex = id / recordsPerBlock;
        int inIndex = id % recordsPerBlock;
        BlockNode* bn = file->getblock(blockIndex);
        int offset = inIndex * recordLen;
        auto record = readRecord(table, bn->Data + offset);
        if (record != nullptr && checkPredicate(table, predicates, record)) {
            consumer(bn, offset, record);
        }
        if (record != nullptr) {
            freeRecord(record);
        }
    }

    void forEachLinear(
            Table& table,
            std::vector<Predicate>& predicates,
            const std::function<bool(BlockNode*, size_t, std::shared_ptr<std::vector<Value>>)>& consumer // (blocknode, offset, record) => (goOn)
    ) {
        // Linear search
        FileNode* file = bufferManager.GetFile(RECORD_FILE(table.tableName));
        size_t recordLen = getRecordSize(table);
        int recordsPerBlock = BLOCKSIZE / recordLen;
        int blockNum = file->getBlockNum();
        bool continue_ = true;
        for (int i = 0; i < blockNum; i++) {
            BlockNode* block = file->getblock(i);
            for (int j = 0; j < recordsPerBlock; j++) {
                size_t offset = j * recordLen;
                char* ptr = block->Data + offset;
                auto re = readRecord(table, ptr);
                if (re == nullptr) { // unvalid
                    continue;
                }

                if (checkPredicate(table, predicates, re)) {
                    // Find !!!
                    continue_ = consumer(block, offset, re);
                    if (!continue_) {
                        freeRecord(re);
                        return;
                    }
                }
                freeRecord(re);
            }
        }
    }
}