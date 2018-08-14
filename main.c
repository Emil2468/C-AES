#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"utils.h"
#include"AES.h"

void EncryptFile(char* fileName, char* newFileName, Block key);
int numberOfBlocks(int fileSize);
int GetFileSize(FILE *file);
void FileToBlocks(FILE *file, int nOfBlocks, Block* emptyBlocks);
Block *blocks;
int main(int argc, char const *argv[])
{   
    
    FILE *file = fopen("Resources/TestDoc", "r");
    int size = GetFileSize(file);
    int nOfBlocks = numberOfBlocks(size);
    blocks = malloc(sizeof(Block) * nOfBlocks);
    FileToBlocks(file, nOfBlocks, blocks);
    // for(int i = 0; i < nOfBlocks; i++) {
    //     //printf("%s\n", blockToHex(*(filledBlocks + i)));
    // }
    return 0;
}

void EncryptFile(char* fileName, char* newFileName, Block key) {
    FILE *fp;
    long int size;
    fp = fopen(fileName, "r");
    if(fp == NULL) {
        printf("File couldn't open\n");
        return;
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp); 
    rewind(fp);
    int n = fgetc(fp);
    while(n != EOF) {
        
    }
    fclose(fp);
}

int GetFileSize(FILE* file) {
    fseek(file, 0L, SEEK_END);
    int size = ftell(file); 
    rewind(file);
    return size;
}

void FileToBlocks(FILE *file, int nOfBlocks, Block* emptyBlocks) {
    int toPad = 0;
    for(int i = 0; i < nOfBlocks; i++) {
        for(int j = 0; j < 128; j++) {
            int n = fgetc(file);
            if(n != EOF) {
                (*(emptyBlocks + i)).bytes[j] = (uchar)n;
            } else {
                toPad = 128 - j; //No more chars, so we need to pad the rest
                break;
            }
        }
    }
    //Pad the remaining part of the block
    for(int i = toPad; i < 128; i++) {
        (*(emptyBlocks + nOfBlocks - 1)).bytes[i] = toPad;
    }
    fclose(file);
}

int numberOfBlocks(int fileSize) {
    int n = fileSize / 128;
    if (128 * n == fileSize) {
        return n;
    } else {
        return n + 1;
    }
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