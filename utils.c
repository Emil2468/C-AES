#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"utils.h"

uchar hexToUchar(char* hex) {
    char hexes[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    uchar fstChar = 0;
    uchar sndChar = 0;
    for(uchar i = 0; i < 16; i++){
        if (*hex == hexes[i]) {
        fstChar = i;
        }
        if (*(hex + 1) == hexes[i]) {
        sndChar = i;
        }
    }
    return (fstChar * 16 + sndChar);
}


char* blockToHex(Block b) {
    char *hex = malloc(32); //string will contain 32 chars
    for(int i = 0; i < 16; i++) {
        char* byte = ucharToHex(b.bytes[i]);
        *(hex + i * 2) = *(byte);
        *(hex + i * 2 + 1) = *(byte + 1);
        free(byte);
    } 
    return(hex);
}

Block hexToBlock(char* hex) {
    Block b;
    for(int i = 0; i < 16; i++) {
        b.bytes[i] = hexToUchar((hex + i * 2));
    }
    return b;
}

char* ucharToHex(uchar value) {
    char* hex = malloc(2); //String will contain 2 chars
    char hexes[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    int n = 0;
    n = (value / 16);
    *(hex) = hexes[n];
    *(hex + 1) = hexes[value - (n * 16)];
    return hex;
}


/* Multiply two numbers in the GF(2^8) finite field defined 
 * by the polynomial x^8 + x^4 + x^3 + x + 1 = 0
 * using the Russian Peasant Multiplication algorithm
 * (the other way being to do carry-less multiplication followed by a modular reduction)
 */
//Copied from Wikipedia :I
uchar gmul(int a, int b) {
	uchar p = 0; /* the product of the multiplication */
	while (a && b) {
            if (b & 1) /* if b is odd, then add the corresponding a to p (final product = sum of all a's corresponding to odd b's) */
                p ^= a; /* since we're in GF(2^m), addition is an XOR */

            if (a & 0x80) /* GF modulo: if a >= 128, then it will overflow when shifted left, so reduce */
                a = (a << 1) ^ 0x11b; /* XOR with the primitive polynomial x^8 + x^4 + x^3 + x + 1 (0b1_0001_1011) – you can change it but it must be irreducible */
            else
                a <<= 1; /* equivalent to a*2 */
            b >>= 1; /* equivalent to b // 2 */
	}
	return p;
}
