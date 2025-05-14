
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
// Log base 2 for unsigned integers
uint32_t log2u(uint32_t x);

// Bit-level helper functions
uint8_t get_bit(uint8_t* data, size_t bit_index);
void set_bit(uint8_t* data, size_t bit_index, uint8_t bit);

// Bit printing utility
void printBits(uint8_t* data, uint32_t lengthInBits);

// Hash function h3: XORs selected slices of m based on bits in A

void h3(uint8_t* m, uint8_t* A, size_t A_bit_offset, size_t s, uint8_t* result, size_t result_bit_offset);

// Main tag generation function 
__declspec(dllexport) void wca_tag(uint32_t seed, uint32_t a, uint32_t b, uint8_t* A, uint8_t* tag_out);
#ifdef __cplusplus
}
#endif