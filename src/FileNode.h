#pragma once
#include <string>
#include <list>

#ifndef F_OK

#define F_OK 0
#endif // F_OK
#include <time.h>
#include "Type.h"
#include "Exception.h"
#include "BlockNode.h"

extern int blockNum;      //use a global varible to record the block number
using namespace std;

class BlockNode;

/* *
* 1. FileNode contain BlockNode
* 2. FileNode and BlockNode are both link structure
* 3. Just use the public functions of the class !
* 4. Normally, CatalogManager And RecordManager Need to get FileNode first, and
        *    use this FileNode to get BlockNode
* 5. !======= File <=> Table ======== !
* 6. While the size of data is not big enough, a FileNode just have a BlockNode
* */

class FileNode {
    string FileName;              // A table maps a File i.e. BookTable -> Book.db
    list<BlockNode *> accessQueue; // store available block
    list<BlockNode *> cacheQueue;  // store recently used block

    //FILE - Object type that identifies a stream and contains the information needed to control it,
    // including a pointer to its buffer, its position indicator and all its state indicators.
    FILE *fp = nullptr;

    char *readBlock(int offset) const;
    void cleanup();
    void writeBack(int offset, char *Data);
    friend class BufferManager;
public:

    FileNode() = default;

    ~FileNode();  //write dirty block to disk
    int getBlockNum() const;   //get this FileNode's block number
    BlockNode *getblock(int index); //get index's block
    int allocNewNode(BlockNode *NewBlock); //add a new block to a fileNode ,return the offset of this block in this fileNode
    void synchronize();                //deal with dirty block
};

