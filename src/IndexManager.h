#pragma once

//#include "IndexManager.h"
#include "BufferManager.h"

#include "Type.h"
#include "BPlusTree.h"
#include "Index.h"
#include "Property.h"
#include <list>
#include <string>
#include <memory>
#include "IndexIterator.h"
#include "LeafPosition.h"
#define BLOCKSIZE 4096

class IndexIterator;

class IndexManager {
public:

    /**
     * Ctor to create IndexManager.
     * @param index: the index
     */
    IndexManager(Index& index);

    /**
     * Destructor of IndexManager.
     */
    ~IndexManager();

    /**
     * Insert an entry with its pointer to Record File.
     * @param newValue: the entry value wrapper.
     * @param indexInRecord: the record index in the Recor File.
     */
    void insertEntry(Value* newValue, int indexInRecord) ;

    /**
     * Delete an entry with its pointer to Record File.
     * @param value: the entry value wrapper.
     * @return: whether the deletion is successful.
     */
    bool deleteEntry(Value* value) ;

    /**
     * Find the entry with the given value, return leaf iterator in the B+ tree.
     * @param index: the index
     * @return: a pointer to IndexIterator from the value found if the value exists,
     * or the position for the value if the value doesn't exist.
     */
    std::shared_ptr<IndexIterator> find(Value* value) ;

    /**
     * Find the minimum or maximum in the B+ tree, return leaf iterator in the B+ tree.
     * @param min: if it's to find the minimum.
     * @return: a pointer to IndexIterator of the minimum / maximum.
     */
    std::shared_ptr<IndexIterator> findMinOrMax(bool min) ;

    /**
     * Static function to create index.
     * @param index: the index
     */
    static void createNewIndex(Index& index);

    /**
     * Static function to judge index file.
     * @param indexName: the index
     */
    static bool hasIndex(std::string indexName);

    /**
     * Static function to drop index.
     * @param index: the index
     */
    static void dropIndex(Index& index);

#ifdef _DEBUG
    void printTree() const;
#endif // _DEBUG
private:
    Index& _index;
    Property* property;
    FileNode* treeFile; // File node of the B+ tree file.
    void* tree; // abstract pointer to a template B+ tree.

    // template method of creating tree with given root index in the file node.
    template<typename T, int size>
    void createTree(int rootIndex);

    // template method of generatring iterator with given position.
    template<typename T>
    std::shared_ptr<IndexIterator> generateIterator(LeafPosition<T, BLOCKSIZE>* position);

    friend class IndexIterator;


};
