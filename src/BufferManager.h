#ifndef BUFFERMANAGER_BUFFERMANAGER_H
#define BUFFERMANAGER_BUFFERMANAGER_H
#include "FileNode.h"
#define BLOCKSIZE 4096           //define BLOCKSIZE 4096 bit
#define CACHESIZE 20000          //the MAX number of block in cache
#define LRU_TIME_VALUE 2         // Last recently used Time
#include <iostream>
#include <string>
#include <list>
#ifndef F_OK
#define F_OK 0
#endif // F_OK
#include <time.h>
#include <cstdio>
#include "Type.h"
#include "Exception.h"
#include "FileNode.h"

extern int blockNum;               //use a global varible to record the block number
using namespace std;

class FileNode;
//BufferManager contains operation about `Memory` and `Disk`
class BufferManager {
private:
    vector<FileNode *> FileServices;
public:
    BufferManager() = default;
    ~BufferManager();
    void globalSynchronize();  // //synchronize all the files in BufferManager's FileServices
    bool JudgeFileExistence(string FileName);    //return true => file exist
    bool CreateFile(string FileName);        //return true => create table sucessfully.
    FileNode *GetFile( string FileName);    //get this FileName's FileNode
    void DeleteFile(const string FileName);      //delete this file
};

//To clarify, using extern tells the compiler that an object of type BM called x exists somewhere.
// It's not the compilers job to know where it exists,
// it just needs to know the type and name so it knows how to use it.
extern BufferManager bufferManager;
#endif //BUFFERMANAGER_BUFFERMANAGER_H