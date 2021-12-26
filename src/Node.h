#include <functional>
#include <list>
#include <memory>
#include <utility>
#include <iostream>
#include "Exception.h"

#pragma once

#define TEMP_DEF template <typename T, int NodeSize>
#define TEMP_NODE Node<T, NodeSize>
//#define TEMP_TREE BPlusTree<T, NodeSize>
#define TEMP_POSITION LeafPosition<T, NodeSize>
#define M ((NodeSize - 4 * sizeof(int) - sizeof(bool)) / (sizeof(T) + sizeof(int)))

TEMP_DEF
struct Node {
    int id;
    bool isInternal;
    int contentSize;
    T value[M];
    int pointers[M + 1]; // If isInternal = false, then pointers[M] points to the next leaf.
    int lastPoint; // If isInternal = false, then lastPoint points to the last leaf.
};