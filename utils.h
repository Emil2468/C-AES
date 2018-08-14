#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef Utils
#define Utils

struct Block
{
    __u_char bytes[16];
};
typedef struct Block Block;
typedef unsigned char uchar;
char* blockToHex(Block b);
Block hexToBlock(char* str);
char* ucharToHex(uchar value);
uchar gmul(int a, int b);

#endif