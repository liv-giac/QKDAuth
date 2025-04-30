#pragma once
#ifndef WCA_TAG_H
#define WCA_TAG_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Platform-specific export macro


// Function prototypes

// Helper: log2 wrapper
uint32_t log2u(uint32_t x);

// Function h3: XOR operation for certain subsets of data
void h3(uint8_t* m, uint8_t* A, size_t s, uint8_t* result);

// Entry point: wca_tag for generating a tag based on provided inputs
__declspec(dllexport) void wca_tag(uint32_t index, uint32_t a, uint32_t b, uint8_t* A, uint8_t* tag_out);

#endif // WCA_TAG_H


