#pragma once

#include "Node.h"
#include "LeafPosition.h"
#define TEMP_DEF template <typename T, int NodeSize>
#define TEMP_NODE Node<T, NodeSize>
//#define TEMP_TREE BPlusTree<T, NodeSize>
#define TEMP_POSITION LeafPosition<T, NodeSize>
#define M ((NodeSize - 4 * sizeof(int) - sizeof(bool)) / (sizeof(T) + sizeof(int)))

//B-tree used for indexing and B+tree used to store the actual records.
// B+tree provides sequential search capabilities in addition to the binary search, which gives the database more control
// to search non-index values in a database.

TEMP_DEF
class BPlusTree {
public:
    BPlusTree(TEMP_NODE *root, std::function<TEMP_NODE *(int)> getChildFunc,
              std::function<void(TEMP_NODE *)> modifyNodeFunc,
              std::function<void(TEMP_NODE *)> deleteNodeFunc,
              std::function<TEMP_NODE *(void)> createNodeFunc,
              std::function<void(int)> onRootChangeFunc)
            : _root(root), _getChildFunc(getChildFunc), _modifyNodeFunc(modifyNodeFunc),
              _deleteNodeFunc(deleteNodeFunc), _createNodeFunc(createNodeFunc), _onRootChangeFunc(onRootChangeFunc) {
    }

    TEMP_POSITION *find( const T& value);

    TEMP_POSITION *findExtreme(bool isMax);

    void insert( const T &value, int pointer);

    bool del(const T &value);

    TEMP_NODE *next(TEMP_NODE *leafNode) const;

    int getM() const { return M; }
#ifdef _DEBUG
    void printTree() const;
#endif // _DEBUG

private:
    TEMP_NODE* _root;
    //Class template std::function is a general-purpose polymorphic function wrapper.
    // Instances of std::function can store, copy, and invoke - functions, lambda expressions, or other function objects,
    // as well as pointers to member functions and pointers to data members.
    std::function<TEMP_NODE*(int)> _getChildFunc;
    std::function<void(TEMP_NODE*)> _modifyNodeFunc;// store a call to a member function
    std::function<void(TEMP_NODE*)> _deleteNodeFunc;
    std::function<TEMP_NODE*(void)> _createNodeFunc;
    std::function<void(int)> _onRootChangeFunc;// store a free function
    std::list<std::shared_ptr<TEMP_POSITION>> _stack;
    int _findPos(T values[], T value, int size) const ;
    void _delInternal();
};

#include "BPlusTree.inl"