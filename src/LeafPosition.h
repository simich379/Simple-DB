#pragma once
#include "Node.h"
#define TEMP_DEF template <typename T, int NodeSize>
#define TEMP_NODE Node<T, NodeSize>
//#define TEMP_TREE BPlusTree<T, NodeSize>
#define TEMP_POSITION LeafPosition<T, NodeSize>
#define M ((NodeSize - 4 * sizeof(int) - sizeof(bool)) / (sizeof(T) + sizeof(int)))

TEMP_DEF
struct LeafPosition {
    TEMP_NODE* node;
    int position;
    LeafPosition(TEMP_NODE* node, int position)
            : node(node), position(position) {}
};