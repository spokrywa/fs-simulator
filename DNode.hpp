//philip cho Sebastian Pokrywa
#ifndef _DNODE_HPP_
#define _DNODE_HPP_

#include <cstdio>

struct DNode{
    DNode(int s, int l, int u){
        start=s;
        end=l;
        usedBit=u;
    }
    int usedBit;  
    int start;
    int end;
};

#endif
