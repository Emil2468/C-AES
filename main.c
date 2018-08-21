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
void DecryptFile(char* fileName, char* newFileName, Block key);
void WriteOneBlockToFile(Block block, FILE *file);
Block RemovePadding(Block b);

int main(int argc, char *argv[])
{   
    if(argc != 5) {
        printf("Wrong number of arguments. Expected the following: key, old file, new file, command (enc or dec)\n");
        return 1;
    } 
    char *argsCopy[argc];
    for(int i = 0; i < argc; i++) {
        argsCopy[i] = argv[i];
    }
    Block key = hexToBlock(argsCopy[1]);
    if (strcmp(argsCopy[4], "enc") == 0) {
        EncryptFile(argsCopy[2], argsCopy[3], key);
    } else if(strcmp(argsCopy[4], "dec") == 0){
        DecryptFile(argsCopy[2], argsCopy[3], key);
    } else {
        printf("Command not understood: %s\n", argsCopy[4]);
        return 1;
    }
    
    return 0;
}


void EncryptFile(char* fileName, char* newFileName, Block key) {
    FILE *file = fopen(fileName, "r");
    FILE *newFile = fopen(newFileName, "w");
    if(file == NULL) {
        printf("ERROR: file not found\n");
        return;
    }
    int size = GetFileSize(file);
    int nOfBlocks = numberOfBlocks(size);
    printf("Blocks needed: %i\n", nOfBlocks);
    Block plainBlock;
    Block encryptedBlock;
    for(int i = 0; i < nOfBlocks; i++) {
        plainBlock = FileToBlock(file);
        encryptedBlock = Encrypt(key, plainBlock);
        WriteOneBlockToFile(encryptedBlock, newFile);
    }
    fclose(file);
    fclose(newFile);
    printf("Done encrypting file\n");
}

void DecryptFile(char* fileName, char* newFileName, Block key) {
    FILE *file = fopen(fileName, "r");
    FILE *newFile = fopen(newFileName, "w");
    if(file == NULL) {
        printf("ERROR: file not found\n");
        return;
    }
    int size = GetFileSize(file);
    int nOfBlocks = numberOfBlocks(size);
    printf("Blocks needed: %i\n", nOfBlocks);
    Block plainBlock;
    Block decryptedBlock;
    for(int i = 0; i < nOfBlocks; i++) {
        plainBlock = FileToBlock(file);
        decryptedBlock = Decrypt(key, plainBlock);
        if(i == nOfBlocks - 1) {
            decryptedBlock = RemovePadding(decryptedBlock);
        }
        WriteOneBlockToFile(decryptedBlock, newFile);
    }
    fclose(file);
    fclose(newFile);
    printf("Done decrypting file\n");
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

void WriteOneBlockToFile(Block block, FILE *file) {
    for(int j = 0; j < 16; j++) {
        fputc((char)block.bytes[j], file);
    }
}

Block RemovePadding(Block b) {
    int counter = 1;
    while(b.bytes[15 - (counter - 1)] == b.bytes[15 - (counter)]) {
        counter++;
    }
    if(counter == b.bytes[15]) { //If this is true, we have found the right amount of padding
        for(int i = 16 - counter; i < 16; i++) {
            b.bytes[i] = 0x00;
        }
    } 
    return b;
}

int GetFileSize(FILE* file) {
    fseek(file, 0L, SEEK_END);
    int size = ftell(file); 
    rewind(file);
    return size;
}

//Reads 16 bytes from file and returns block containing those, does not close file after
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