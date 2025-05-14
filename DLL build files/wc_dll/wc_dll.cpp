#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pch.h"
#include <iostream>
#include <iomanip>
#include "wc_dll.h"

#define BITS_TO_BYTES(bits) (((bits) + 7) / 8)
#ifdef __cplusplus
extern "C" {
#endif
static uint32_t log2u(uint32_t x) {
    uint32_t res = 0;
    while (x >>= 1) res++;
    return res;
}

uint8_t get_bit(uint8_t* data, size_t bit_index) {
    return (data[bit_index / 8] >> (7 - (bit_index % 8))) & 1;
}

void set_bit(uint8_t* data, size_t bit_index, uint8_t bit) {
    if (bit)
        data[bit_index / 8] |= (1 << (7 - (bit_index % 8)));
    else
        data[bit_index / 8] &= ~(1 << (7 - (bit_index % 8)));
}

void printBits(uint8_t* data, uint32_t lengthInBits) {
    for (uint32_t i = 0; i < lengthInBits; ++i) {
        std::cout << (int)get_bit(data, i);
        if ((i + 1) % 8 == 0) std::cout << " ";  // for easier reading
    }
    std::cout << "\nTotal bits: " << lengthInBits << std::endl;
}


void h3(uint8_t* m, uint8_t* A, size_t A_bit_offset, size_t s, uint8_t* result, size_t result_bit_offset) {
    for (size_t k = 0; k < 2 * s; k++) {

        bool bit = get_bit(A, A_bit_offset + k);

        if (bit) { //only xors if they key's k-th bit is 1
            for (size_t j = 0; j < s; j++) {

                uint8_t m_bit = get_bit(m, k * s + j);
                uint8_t current_bit = get_bit(result, j + result_bit_offset);

                uint8_t new_bit = current_bit ^ m_bit; //xor

                set_bit(result, j + result_bit_offset, new_bit);
            }
        }
    }
}

__declspec(dllexport) void  wca_tag(uint32_t seed, uint32_t a, uint32_t b, uint8_t* A, uint8_t* tag_out) {
    //calculating s, sequence length and n of substrings
    uint32_t s = (uint32_t)(b + log2(log2((double)a)));
	std::cout << "s: " << s << std::endl;
    uint32_t sequence_length = log2u(a) - log2u(b);
    size_t substrings = (a + (2 * s - 1)) / (2 * s);

    srand(seed); //sets random PNRG to seed

    //padding key vector
    uint32_t padded_len = 2 * s * substrings;
    uint8_t* A_padded = (uint8_t*)calloc(BITS_TO_BYTES(padded_len), 1);
    memcpy(A_padded, A, BITS_TO_BYTES(a));

    //allocating F
    uint8_t* F = (uint8_t*)calloc(BITS_TO_BYTES(s * substrings), 1);
    uint8_t* A_substr = A_padded;

    //allocating m and generating random bits
    uint8_t* m = (uint8_t*)calloc(BITS_TO_BYTES(2*s*s), 1);
    for (size_t i = 0; i < 2*s*s; ++i) {
        uint8_t bit = rand() % 2;
        size_t byte_index = i / 8;
        size_t bit_index = 7 - (i % 8);
        m[byte_index] |= (bit << bit_index);
    }

    //for every iteration
    for (size_t i = 0; i < sequence_length; i++) {

        //for every substring
        for (size_t z = 0; z < substrings; z++) {
			//calculating bit offsets
            size_t A_bit_offset = z * 2 * s;
            size_t result_bit_offset = z * s;
            //hashing, takes care of saving output in F
            h3(m, A_substr, A_bit_offset, s, F, result_bit_offset);
        }

		//if last sequence
        if (i < sequence_length - 1) {
            //recalculate substrings
            substrings = (s * substrings + (2 * s - 1)) / (2 * s);
			//reallocate F
			memcpy(A_substr, F, BITS_TO_BYTES(s * substrings));
            //set F to 0
            for (size_t j = 0; j < s* substrings; j++) {
                set_bit(F, j, 0);
            }
        }
    }

    size_t tag_start_bit = s - b;
    for (size_t i = 0; i < b; i++) {
        uint8_t bit = get_bit(F, tag_start_bit + i);
        set_bit(tag_out, i, bit);
    }

    size_t full_bytes = b / 8;
    size_t remaining_bits = b % 8;
    if (remaining_bits > 0) {
        uint8_t last_mask = 0xFF << (8 - remaining_bits);
        tag_out[full_bytes] &= last_mask;
    }

    

    free(A_padded);
    free(m);
    free(F);
    std::cout << "Final tag: ";
    printBits(tag_out, b);
}
#ifdef __cplusplus
}
#endif