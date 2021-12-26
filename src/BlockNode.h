#ifndef SIMPLEDBMS_BLOCKNODE_H
#define SIMPLEDBMS_BLOCKNODE_H

#include "BufferManager.h"

using namespace std;
//BlockNode are both link structure
struct BlockNode {
    char *Data = nullptr;
    bool dirty = false; // dirty bit == modified bit show if the block has been modified and has not been saved to storage yet
                        // When the block.dirty == true(a block of memory is  replaced), this block need to write back to disk
    int offset;                 //offset in FileNode
    bool pin = false; // pin(read-in) a Blocknode
    char flag = 0;                  //the priority of this BlockNode
    string FileName;              //Which file this block belongs to
};

#endif //SIMPLEDBMS_BLOCKNODE_H
