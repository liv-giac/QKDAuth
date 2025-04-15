// testingWC.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include "wc_dll.h"
#define BITS_TO_BYTES(bits) (((bits) + 7) / 8)
int main()
{

    //FIRST TEST
    
    //lengths given in bits
    uint32_t a = 5;
    uint32_t b = 2;
    
    //initial A value: 01001000
    uint8_t A[1]= { 0x48};
    //allocating for first tag
    uint8_t* tag_out_small = (uint8_t*)calloc(BITS_TO_BYTES(b), 1);
    //setting seed as 1 in first test without using known seed vector
    uint32_t seed = 1;
    //running tag function for first test
    wca_tag(seed, a, b, A, tag_out_small);

    
    free(tag_out_small);



    ////SECOND TEST


	//lengths given in bits,
    
    a = 640;  
    b = 128;

    //common key
    
    uint8_t k[80] = {
    0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C,
    0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x22, 0x33, 0x44,
    0x55, 0x66, 0x77, 0x88,
    0x99, 0xAA, 0xBB, 0xCC,
    0xDD, 0xEE, 0xFF, 0x00
};

    //given index to seed, common seed vector allocated
    uint32_t v[30] = {  // 30 seeds
    0x01020304, 0x05060708, 0x090A0B0C,
    0x11121314, 0x15161718, 0x191A1B1C,
    0x21222324, 0x25262728, 0x292A2B2C,
    0x31323334, 0x35363738, 0x393A3B3C,
    0x41424344, 0x45464748, 0x494A4B4C,
    0x51525354, 0x55565758, 0x595A5B5C,
    0x61626364, 0x65666768, 0x696A6B6C,
    0x71727374, 0x75767778, 0x797A7B7C,
    0x81828384, 0x85868788, 0x898A8B8C,
    0x91929394, 0x95969798, 0x999A9B9C
    }; 
    uint32_t index = 7;
    seed = v[index];


    //allocated space for generated tag
    uint8_t* tag_out = (uint8_t*)calloc(BITS_TO_BYTES(b), 1);
    
    //tag generation for second test
    wca_tag(seed, a, b, k, tag_out);

    
    free(tag_out);
    return 0;
}
