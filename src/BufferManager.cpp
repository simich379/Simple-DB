
#include <unistd.h>
#include "BufferManager.h"


/*
 *  CatalogManager -> .struct file
 *  RecordManager  -> .db file
 */

BufferManager bufferManager = BufferManager();

int blockNum = 0;

BufferManager::~BufferManager() {
    for (auto FN : FileServices) {
        fclose(FN->fp);
    }
};
//================ Struct part funcion===================
//struct Function interface with CatalogManager
bool BufferManager::CreateFile(string FileName) {
    FILE *fp = fopen(FileName.c_str(), "wb");
    if (fp == nullptr)
        throw SQLException("Create file error!");

    //create Record file at the same time
    auto *FN = new FileNode;
    FN->FileName = FileName;
    FN->fp = fp;
    this->FileServices.emplace_back(FN); //Inserts a new element at the end of the vector, right after its current last element.

}

bool BufferManager::JudgeFileExistence(string FileName) {
    auto iter = this->FileServices.begin();
    while (iter != this->FileServices.end()) {
        auto item = *iter;
        if (FileName == item->FileName)
            return true;
        iter++;
    }

    return access(FileName.c_str(), F_OK) != -1;
}


//================ Content part funcion===================
//Content Function interface with RecordManager
FileNode *BufferManager::GetFile( string FileName) {
    auto iter = this->FileServices.begin();   //a struct like LRU to read Record in memory
    while (iter != this->FileServices.end()) {
        auto item = *iter;
        if (FileName == item->FileName)
            return item;
        iter++;
    }

    FILE *fp = fopen(FileName.c_str(), "rb+");
    if (fp == nullptr) throw SQLException("Didn't create this file yet.");
    auto *FN = new(FileNode);
    FN->FileName = FileName;
    FN->fp = fp;
    this->FileServices.emplace_back(FN);//Inserts a new element at the end of the vector, right after its current last element.
    return FN;
}

void BufferManager::globalSynchronize(){  //synchronize all the files in BufferManager's FileServices
    for (auto FN : FileServices)
        FN->synchronize();

}

void BufferManager::DeleteFile(const string FileName) {
    //need to delete both struct file and db file
    auto iter = this->FileServices.begin();   //a struct like LRU to read Record in memory
    while (iter != this->FileServices.end()) {
        auto item = *iter;
        if (FileName == item->FileName) {
            fclose(item->fp);
            this->FileServices.erase(iter);
            break;
        }
        iter++;
    }

    if (remove(FileName.c_str()) != 0) throw SQLException("Delete File error!");
}