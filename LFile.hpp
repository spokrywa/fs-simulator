//philip cho Sebastian Pokrywa
#ifndef _LFILE_HPP_
#define _LFILE_HPP_

#include <list>
#include <math.h>
#include "FNode.hpp"
#include "LDisk.hpp"

class LFile
{
    public:
        LFile(int ts, int bs,LDisk * d);
        void append(int numBytes);
        void remove(int numBytes);
        void print();
        int getSize();
        ~LFile();
    private:
        std::list<FNode*> lfile;
        int totalSize;
        int blockSize;
        LDisk * disk;

};

#endif
