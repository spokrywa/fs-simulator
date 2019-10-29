//philip cho Sebastian Pokrywa
#include "LDisk.hpp"

LDisk::LDisk(int blocks){
    DNode* head = new DNode(0, blocks, 0);
    disk.push_back(head);
    totalBlocks = blocks;
}

int LDisk::insert(){
    int blockID = 0;    
    //traverse ldisk to allocate a block
    for(std::list<DNode*>::iterator it=disk.begin(); it != disk.end(); ++it){
        if(!(*it)->usedBit){
            //split off new node with allocate bit set
            blockID = (*it)->start;
            (*it)->start = (*it)->start-1;
            DNode* newNode = new DNode(blockID, blockID, 1);
            disk.insert(it, newNode);
            return blockID;
        }
    } 
    return blockID;
}

void LDisk::remove(int blockAddress){
    //if all blocks free, can't remove anything so just return
    if(disk.size() == 1 && !disk.front()->usedBit){
        std::cout << "All disk space free, nothing to remove";
        return;
    }

    //traverse ldisk to find block address
    for(std::list<DNode*>::iterator it=disk.begin(); it != disk.end(); ++it){
        if(blockAddress >= (*it)->start && blockAddress <= (*it)->end){
            if(!(*it)->usedBit){
                std::cout << "Requested block address is already free";
                return;
            }
            //if node is only one block, set it to free
            if((*it)->start == (*it)->end){
                (*it)->usedBit = 0;
                return;
            }else{ 
                if((*it)->start == blockAddress){       // special cases
                    (*it)->start = blockAddress+1;
                    DNode* newNode = new DNode(blockAddress, blockAddress, 0);
                    if(blockAddress == 0)
                        disk.push_front(newNode);
                    else
                        disk.insert(it, newNode);
                    return;
                }else if((*it)->end == blockAddress){
                    (*it)->end = blockAddress-1;
                    DNode* newNode = new DNode(blockAddress, blockAddress, 0);
                    disk.insert(++it, newNode);
                    return;
                }else{ //splitting node and inserting a new one in between
                    DNode* newNode = new DNode(blockAddress, blockAddress, 0);
                    DNode* splitNode = new DNode(blockAddress+1, (*it)->end, 1);
                    (*it)->end = blockAddress-1;
                    disk.insert(++it, newNode);
                    disk.insert(++it, splitNode);
                    return;
                }
            }
        }
    }
}

void LDisk::update(){
    for(std::list<DNode*>::iterator it=disk.begin(); it != disk.end(); ++it){
        auto prev = *std::prev(it);
        auto next = *std::next(it);
        //find continuous blocks, checking for 3 first and then the cases of 2 blocks
        if((*it)->usedBit == prev->usedBit && (*it)->usedBit == next->usedBit){
            (*it)->start = prev->start;
            (*it)->end = next->end;
            delete prev;
            delete next;
            disk.erase(std::prev(it));
            disk.erase(std::next(it));            
            return;
        }else if((*it)->usedBit == prev->usedBit){
            (*it)->start = prev->start;
            delete prev;
            disk.erase(std::prev(it));
            return;
        }else if((*it)->usedBit == next->usedBit){
            (*it)->end = next->end;
            delete next;
            disk.erase(std::next(it));
            return;
        }
    }
}

void LDisk::print(int blockSize){
    for(std::list<DNode*>::iterator it=disk.begin(); it != disk.end(); ++it){
        if((*it)->usedBit)
            std::cout << "In use: " << (*it)->start << '-' << (*it)->end << std::endl;
        else
            std::cout << "Free: " << (*it)->start << '-' << (*it)->end << std::endl;
    }
}

LDisk::~LDisk(){
    while(!disk.empty()) delete disk.front(), disk.pop_front();
}