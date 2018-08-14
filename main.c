#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"utils.h"
#include"AES.h"

void EncryptFile(char* fileName, char* newFileName, Block key);
int numberOfBlocks(int fileSize);
int GetFileSize(FILE *file);
Block FileToBlock(FILE *file);
void PrintBytesInBlock(Block b);
void WriteBlocksToFile(Block blocks[], int nOfBlocks, char* fileName);

int main(int argc, char const *argv[])
{   
    Block key = hexToBlock("12e3f7817e8a6b5c6f5a436e7b9c9e8a");
    EncryptFile("Resources/TestDoc", "Resources/EncryptedDoc", key);
    return 0;
}

void EncryptFile(char* fileName, char* newFileName, Block key) {
    FILE *file = fopen("Resources/TestDoc", "r");
    int size = GetFileSize(file);
    int nOfBlocks = numberOfBlocks(size);
    printf("%i blocks needed\n", nOfBlocks);
    Block blocks[nOfBlocks];
    for(int i = 0; i < nOfBlocks; i++) {
        blocks[i] = FileToBlock(file);
    }
    // for(int i = 0; i < nOfBlocks; i++) {
    //     printf("%s\n", blockToHex(blocks[i]));
    // }
    Block encryptedBlocks[nOfBlocks];
    for(int i = 0; i < nOfBlocks; i++) {
        encryptedBlocks[i] = Encrypt(key, blocks[i]);
    }
    WriteBlocksToFile(encryptedBlocks, nOfBlocks, newFileName);
}

void WriteBlocksToFile(Block blocks[], int nOfBlocks, char* fileName) {
    FILE *file = fopen(fileName, "w");
    for(int i = 0; i < nOfBlocks; i++) {
        for(int j = 0; j < 16; j++) {
            fputc((char)blocks[i].bytes[j], file);
        }
    }
    fclose(file);
}

int GetFileSize(FILE* file) {
    fseek(file, 0L, SEEK_END);
    int size = ftell(file); 
    rewind(file);
    return size;
}

//Reads 128 bytes from file and returns block containing those, does not close file after
Block FileToBlock(FILE *file) {
    int toPad = 0;
    Block block;
    //printf("%i\n", ftell(file));
    for(int i = 0; i < 16; i++) {
        int n = fgetc(file);
        if(n != EOF) {
            block.bytes[i] = (uchar)n;
        } else {
            toPad = 16 - i; //No more chars, so we need to pad the rest
            break;
        }
    }
    //Pad the remaining part of the block
    for(int i = 16 - toPad; i < 16; i++) {
       block.bytes[i] = toPad;
    }
    return block;
}

int numberOfBlocks(int fileSize) {
    int n = fileSize / 16;
    if (16 * n == fileSize) {
        return n;
    } else {
        return n + 1;
    }
}

void PrintBytesInBlock(Block b) {
    for(int i = 0; i < 16; i++) {
        printf("%i, ", b.bytes[i]);
    }
    printf("\n");
}
void EncryptionTest() {
    char* keyChars = "000102030405060708090a0b0c0d0e0e";
    char* input = "00112233445566778899aabbccddeefe";
    Block key = hexToBlock(keyChars);
    Block plainText = hexToBlock(input);
    Block chiferText = Encrypt(key, plainText);
    Block decrypted = Decrypt(key, chiferText);

    printf("Input: %s\n", input);
    printf("Chifer text: %s\n", blockToHex(chiferText));
    printf("Decrypted: %s\n", blockToHex(decrypted));
}