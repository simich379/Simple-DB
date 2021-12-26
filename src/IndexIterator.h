#pragma once


//#include "IndexManager.h"
#include "BufferManager.h"

#include "Type.h"
#include "BPlusTree.h"

#include <list>
#include <string>
#include <memory>
#include "IndexManager.h"
//#include "IndexIterator.h"

class IndexManager;

class IndexIterator {
public:
    Value* currentValue; // value of the current iterator
    int indexInRecord; // index pointer of the current iterator
    std::shared_ptr<IndexIterator> next(); // to iterate
   ~IndexIterator();
private:
    void* leafPosition; // abstract pointer to a template class LeafPosition<>
    IndexManager* indexManager;


    // Internal template function of iterator
    template <typename T>
    std::shared_ptr<IndexIterator> generateNext();

    friend class IndexManager;

};
