
#include <stdint.h>
#include <stddef.h> // for size_t



    // Encrypts plain_text using ChaCha20 and authenticates with Poly1305
    // Outputs cipher_text and a 16-byte MAC
    __declspec(dllexport) void  polychacha_encrypt(
        uint8_t* cipher_text,
        uint8_t mac[16],
        uint8_t key[32],
        uint8_t nonce[12],
        uint8_t* ad, uint32_t ad_size,
        uint8_t* plain_text, uint32_t text_size);

    // Decrypts cipher_text if MAC is valid; writes to plain_text
    // Returns 0 on success, -1 on failure (MAC mismatch or allocation failure)
    __declspec(dllexport) int  polychacha_decrypt(
        uint8_t* plain_text,
        uint8_t* cipher_text,
        uint32_t text_size,
        uint8_t mac[16],
        uint8_t key[32],
        uint8_t nonce[12],
        uint8_t* ad, uint32_t ad_size);
