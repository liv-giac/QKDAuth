#include <iostream>
#include <iomanip>  // for std::hex
#include <cstring>  // for memset, strlen
#include "wc_dll.h"
#include "my_mono.h"

#define BITS_TO_BYTES(bits) (((bits) + 7) / 8)

void print_hex(const char* label, const unsigned char* data, size_t len) {
    std::cout << label;
    for (size_t i = 0; i < len; ++i)
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)data[i];
    std::cout << std::dec << std::endl;
}

int main()
{
    // First test (WCA)
    uint32_t a = 5, b = 2;
    uint8_t A[1] = { 0x48 };
    uint8_t* tag_out_small = (uint8_t*)calloc(BITS_TO_BYTES(b), 1);
    uint32_t seed = 1;
    wca_tag(seed, a, b, A, tag_out_small);
    free(tag_out_small);

    // Second test (WCA)
    a = 640; b = 128;
    uint8_t k[80] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
    };
    uint32_t v[30] = {
        0x01020304, 0x05060708, 0x090A0B0C, 0x11121314, 0x15161718, 0x191A1B1C,
        0x21222324, 0x25262728, 0x292A2B2C, 0x31323334, 0x35363738, 0x393A3B3C,
        0x41424344, 0x45464748, 0x494A4B4C, 0x51525354, 0x55565758, 0x595A5B5C,
        0x61626364, 0x65666768, 0x696A6B6C, 0x71727374, 0x75767778, 0x797A7B7C,
        0x81828384, 0x85868788, 0x898A8B8C, 0x91929394, 0x95969798, 0x999A9B9C
    };
    seed = v[7];
    uint8_t* tag_out = (uint8_t*)calloc(BITS_TO_BYTES(b), 1);
    wca_tag(seed, a, b, k, tag_out);
    free(tag_out);

    // ChaCha20-Poly1305 Test
    const char* plaintext = "ciao";
    size_t message_len = strlen(plaintext);
    unsigned char key[32];
    unsigned char nonce[12];
    unsigned char aad[] = "";
    size_t aad_len = strlen((char*)aad);  // Changed to strlen to match encryption and decryption
    unsigned char* ciphertext = new unsigned char[message_len];
    unsigned char mac[16];
    unsigned char* decrypted = new unsigned char[message_len+1];
    decrypted[message_len] = '\0';  // Null-terminate the decrypted string
    memset(key, 0x00, sizeof(key));
    memset(nonce, 0x00, sizeof(nonce));
    std::cout << "--- Encryption Inputs ---" << std::endl;
    print_hex("Key:       ", key, sizeof(key));
    print_hex("Nonce:     ", nonce, sizeof(nonce));
    print_hex("AAD:       ", aad, aad_len);
    print_hex("Plaintext: ", reinterpret_cast<const unsigned char*>(plaintext), message_len);
    std::cout << "Ciphertext size: " << message_len << std::endl;
    print_hex("Ciphertext: ", ciphertext, message_len);
    // Encrypt
    polychacha_encrypt(
        ciphertext,
        mac,
        key,
        nonce,
        aad, aad_len,
        reinterpret_cast<const unsigned char*>(plaintext), message_len
    );


    // Debugging: Print encrypted values
    print_hex("Ciphertext: ", ciphertext, message_len);
    print_hex("MAC: ", mac, sizeof(mac));
    std::cout << std::endl;
    // Decrypt
    std::cout << "--- Decryption Inputs ---" << std::endl;
    print_hex("Key:        ", key, sizeof(key));
    print_hex("Nonce:      ", nonce, sizeof(nonce));
    print_hex("AAD:        ", aad, aad_len);
    print_hex("Ciphertext: ", ciphertext, message_len);
    print_hex("MAC:        ", mac, sizeof(mac));

    int boh = polychacha_decrypt(
        decrypted,
        ciphertext, message_len,
        mac,
        key, 
        nonce,
        aad, aad_len
    );
    std::cout << boh << std::endl;
    std::cout << "Decryption successful!" << std::endl;

    std::cout << std::endl;
    // Output results
    std::cout << "Original: " << plaintext << std::endl;
    print_hex("Encrypted (hex): ", ciphertext, message_len);

    std::cout << "Decrypted: " << decrypted << std::endl;

    delete[] ciphertext;
    delete[] decrypted;
    return 0;
}
