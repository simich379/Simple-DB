#include "IndexIterator.h"
#include "LeafPosition.h"
#include "CharWrapper.h"
#include "BPlusTree.h"

IndexIterator::~IndexIterator()
{

    BaseType type = indexManager->property->type.getBaseType();
    switch (type) {
        case BaseType::FLOAT:
            delete (LeafPosition<double, BLOCKSIZE>*)leafPosition;
            break;
        case BaseType::INT:
            delete (LeafPosition<int, BLOCKSIZE>*)leafPosition;
            break;
        case BaseType::BOOL:
            delete (LeafPosition<bool, BLOCKSIZE>*)leafPosition;
            break;
        case BaseType::VARCHAR:
            delete (LeafPosition<CharWrapper, BLOCKSIZE>*)leafPosition;
            break;
    }
    delete currentValue;

}

std::shared_ptr<IndexIterator> IndexIterator::next()
{
    BaseType type = indexManager->property->type.getBaseType();
    switch (type) {
        case BaseType::FLOAT:
            return generateNext<double>();
        case BaseType::INT:
            return generateNext<int>();
        case BaseType::BOOL:
            return generateNext<bool>();
        case BaseType::VARCHAR:
            return generateNext<CharWrapper>();
    }
    throw SQLException("Unknown type");
}

template <typename T>
std::shared_ptr<IndexIterator> IndexIterator::generateNext()
{
    auto tree = (BPlusTree<T, BLOCKSIZE>*)indexManager->tree;
    auto pos = (LeafPosition<T, BLOCKSIZE>*) leafPosition;
    auto treeNode = pos->node;
    int treePosition = pos->position;
    if (treePosition < treeNode->contentSize - 1) {
        return indexManager->generateIterator(new LeafPosition<T, BLOCKSIZE>(
                treeNode, treePosition + 1));

    }
    if (treeNode->pointers[tree->getM()] > 0) {
        treeNode = tree->next(treeNode);
        return indexManager->generateIterator(new LeafPosition<T, BLOCKSIZE>(
                treeNode, 0));
    }
    return (std::shared_ptr<IndexIterator>)nullptr;
}