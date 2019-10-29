//philip cho Sebastian Pokrywa
#ifndef _FNode_HPP_
#define _FNode_HPP_
#include <cstdio>

struct FNode{
    int blockAddress;

    FNode()
    {
        blockAddress = -1;
    }

    FNode(int m)
    {
        blockAddress = m;
    }
};


#endif
