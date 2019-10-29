//philip cho Sebastian Pokrywa
#ifndef _LDisk_HPP_
#define _LDisk_HPP_

#include <iostream>
#include <list>
#include <iterator>
#include "DNode.hpp"

class LDisk{
    public:
        LDisk(int blocks);
        int insert();
        void remove(int blockAddress);
        void update();
        void print(int blockSize);
        int totalBlocks;
        ~LDisk();
    private:
        std::list <DNode*> disk;
};


#endif
