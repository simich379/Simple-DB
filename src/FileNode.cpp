
#include "FileNode.h"

 char *FileNode::readBlock(int offset) const {
     // obtain file size:
    fseek(this->fp, offset * BLOCKSIZE, SEEK_SET);
     // allocate memory to contain the whole file:
    auto *Data = new char[BLOCKSIZE];
     // copy the file into the buffer:
    fread(Data, BLOCKSIZE, 1, fp);
    return Data;
}

void FileNode::cleanup() {
    auto iter = accessQueue.begin();
    while (iter != accessQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data); // When the block.dirty == true(a block of memory is  replaced/ modified),
                                                        // this block need to write back to disk
        }
        delete item->Data;
        item->Data=nullptr;
        delete item;
        iter = accessQueue.erase(iter);
        blockNum--;
    }
    if (accessQueue.size() <= CACHESIZE / 3) { //if accessQueue is not small enough
        iter = cacheQueue.begin();
        while (iter != cacheQueue.end()) {
            auto item = *iter;
            if (!item->pin) { //didn't delete the pin's block
                if (item->dirty) {
                    this->writeBack(item->offset, item->Data);
                }
                delete item->Data;
                delete item;
                iter = cacheQueue.erase(iter);
                blockNum--;
            }
        }
    }
}

void FileNode::writeBack(int offset, char *Data) {
    fseek(this->fp, offset * BLOCKSIZE, SEEK_SET);
    fwrite(Data, BLOCKSIZE, 1, this->fp);
    fflush(this->fp); // clear  the output buffer and move the buffered data to the disk
}

FileNode::~FileNode() {
    auto iter = accessQueue.begin();
    while (iter != accessQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data);
        }

        delete item;
        iter = accessQueue.erase(iter);
        blockNum--;
    }

    iter = cacheQueue.begin();
    while (iter != cacheQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data);
        }
        delete item;
        iter = cacheQueue.erase(iter);
        blockNum--;
    }
}

int FileNode::getBlockNum() const {
    fseek(this->fp, 0, SEEK_END);
    auto offset_char = (int) ftell(this->fp);
    int sum;
    sum = offset_char / BLOCKSIZE;
    return sum;
}

BlockNode *FileNode::getblock(int index) { //Use LRU to store recently used block
    //cacheQueue has the highest ring to be read
    auto iter = cacheQueue.begin();
    while (iter != cacheQueue.end()) {
        auto item = *iter;
        if (item->offset == index) {
            item->flag = (char) time(nullptr);  // refresh this block's time
            cacheQueue.push_front(item);//insert an element at the front of a list
            cacheQueue.erase(iter);
            return item;
        }
        iter++;
    }

    iter = accessQueue.begin();
    while (iter != accessQueue.end()) {
        auto item = *iter;
        if (item->offset == index) {
            item->flag++;
            if (item->flag == LRU_TIME_VALUE || item->pin) { //add block to cacheQueue depends on pin(read-in) and last used time
                cacheQueue.push_front(item);
                accessQueue.erase(iter);
            } else {
                accessQueue.push_front(item); //insert an element at the front of a list
                accessQueue.erase(iter);
            }
            return item;
        }
        iter++;
    }
    if (blockNum >= CACHESIZE) {
        cleanup();
    }
    //if can't get this block from memory,then read from disk
    blockNum++;
    auto *item = new BlockNode;
    item->offset = index;
    item->dirty = false;
    item->flag = 0;
    item->Data = this->readBlock(index);
    accessQueue.push_front(item);
    return item;
}

int FileNode::allocNewNode(BlockNode *NewBlock) {
    fseek(this->fp, 0, SEEK_END);  // get the end of the fp, start to write new block
    auto offset_char = (int) ftell(this->fp);
    fwrite(NewBlock->Data, BLOCKSIZE, 1, this->fp);
    fflush(this->fp);
    NewBlock->FileName = this->FileName;
    NewBlock->offset = offset_char / BLOCKSIZE;

    accessQueue.push_front(NewBlock);
    blockNum++;
    return offset_char / BLOCKSIZE;
}

void FileNode::synchronize() {   //write dirty block to disk
    auto iter = accessQueue.begin();
    while (iter != accessQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data);
            item->dirty = false;
        }
        iter++;
    }
    iter = cacheQueue.begin();
    while (iter != cacheQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data);
            item->dirty = false;
        }
        iter++;
    }
}
