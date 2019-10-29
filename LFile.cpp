//philip cho Sebastian Pokrywa
#include "LFile.hpp"

LFile::LFile(int ts, int bs, LDisk* d){
    totalSize = ts;
    blockSize = bs;
    disk = d;
    int blockAddr = 0;
    //allocate block by block and append to ldisk 
    for(int i = 0; i < ceil(ts/bs); i++){
        blockAddr =  (*disk).insert() * bs;
        (*disk).update();
        FNode* newNode = new FNode(blockAddr);
        lfile.push_back(newNode);
    }
}

void LFile::append(int numBytes){
    int freeBytes = blockSize - totalSize%blockSize;
    if(numBytes <= freeBytes){ //no need to allocate more disk blocks
        totalSize += numBytes;
        return;
    }else{
        //allocate another block
        int blockAddr =  (*disk).insert() * blockSize;
        (*disk).update();
        FNode* newNode = new FNode(blockAddr);
        lfile.push_back(newNode);
        totalSize += numBytes;
        return;
    }
}

void LFile::remove(int numBytes){
    if(numBytes < blockSize){ //no need to remove node
        totalSize -= numBytes;
        return;
    }else{
        //deallocate as many blocks as needed
        for(int i = 0; i < numBytes/blockSize; i++){
            (*disk).remove(lfile.back()->blockAddress * blockSize);
            (*disk).update();
        }
        totalSize -= numBytes;
        return;
    }
}

int LFile::getSize(){
    return totalSize;
}

void LFile::print(){
    std::cout << "File Size: " << totalSize << std::endl;
    std::cout << "Block Addresses: ";
    for(std::list<FNode*>::iterator it=lfile.begin(); it != lfile.end(); ++it){
        std::cout << (*it)->blockAddress << " -> ";
    }
    std::cout << std::endl;
    
}

LFile::~LFile(){
    while(!lfile.empty()) delete lfile.front(), lfile.pop_front();
}