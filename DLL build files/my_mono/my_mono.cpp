#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "monocypher.h"


#include <iostream>
#include <iomanip>

#ifdef __cplusplus
extern "C" {
#endif


    __declspec(dllexport) void polychacha_encrypt(
        uint8_t* cipher_text,
        uint8_t mac[16],
        uint8_t key[32],
        uint8_t nonce[12],
        uint8_t* ad, uint32_t ad_size,
        uint8_t* plain_text, uint32_t text_size)
    {
#ifdef __cplusplus
        std::cout << "[Encrypt] Starting encryption...\n";
#endif

        uint8_t poly_key[32];
        uint8_t chacha_input[16] = { 0 };
        crypto_chacha20_h(poly_key, key, nonce);

#ifdef __cplusplus
        std::cout << "[Encrypt] Poly1305 key: ";
        for (int i = 0; i < 32; ++i) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)poly_key[i];
        std::cout << std::dec << "\n";
#endif

        crypto_chacha20_ietf(cipher_text, plain_text, text_size, key, nonce, 1);

        uint32_t total_mac_data_size = ad_size + ((16 - ad_size % 16) % 16) +
            text_size + ((16 - text_size % 16) % 16) + 16;
        uint8_t* mac_data = (uint8_t*)malloc(total_mac_data_size);
        uint8_t* p = mac_data;

        memcpy(p, ad, ad_size);
        p += ad_size;
        memset(p, 0, ((16 - ad_size % 16) % 16));
        p += ((16 - ad_size % 16) % 16);

        memcpy(p, cipher_text, text_size);
        p += text_size;
        memset(p, 0, ((16 - text_size % 16) % 16));
        p += ((16 - text_size % 16) % 16);

        uint64_t ad_len = ad_size;
        uint64_t ct_len = text_size;
        memcpy(p, &ad_len, 8); p += 8;
        memcpy(p, &ct_len, 8); p += 8;

        crypto_poly1305(mac, mac_data, total_mac_data_size, poly_key);

#ifdef __cplusplus
        std::cout << "[Encrypt] Computed MAC: ";
        for (int i = 0; i < 16; ++i) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)mac[i];
        std::cout << std::dec << "\n";
#endif

        crypto_wipe(poly_key, sizeof(poly_key));
        crypto_wipe(mac_data, total_mac_data_size);
        free(mac_data);
    }



    __declspec(dllexport) int polychacha_decrypt(
        uint8_t* plain_text,
        uint8_t* cipher_text,
        uint32_t text_size,
        uint8_t mac[16],
        uint8_t key[32],
        uint8_t nonce[12], // 96-bit nonce
        uint8_t* ad, uint32_t ad_size)
    {
        // Step 1: Generate one-time Poly1305 key using ChaCha20 block 0
        uint8_t poly_key[32];
        uint8_t chacha_input[16] = { 0 };
        crypto_chacha20_h(poly_key, key, nonce); // same as encrypt

        // Step 2: Prepare the Poly1305 input (same as in encrypt)
        uint32_t total_mac_data_size = ad_size + ((16 - ad_size % 16) % 16) +
            text_size + ((16 - text_size % 16) % 16) + 16;
        uint8_t* mac_data = (uint8_t*)malloc(total_mac_data_size);
        if (!mac_data) return total_mac_data_size; // allocation failure

        uint8_t* p = mac_data;

        // Copy associated data
        memcpy(p, ad, ad_size);
        p += ad_size;
        memset(p, 0, ((16 - ad_size % 16) % 16));
        p += ((16 - ad_size % 16) % 16);

        // Copy ciphertext
        memcpy(p, cipher_text, text_size);
        p += text_size;
        memset(p, 0, ((16 - text_size % 16) % 16));
        p += ((16 - text_size % 16) % 16);

        // Copy lengths
        uint64_t ad_len = ad_size;
        uint64_t ct_len = text_size;
        memcpy(p, &ad_len, 8); p += 8;
        memcpy(p, &ct_len, 8); p += 8;

        // Step 3: Recompute and verify MAC
        uint8_t computed_mac[16];
        crypto_poly1305(computed_mac, mac_data, total_mac_data_size, poly_key);

        int valid = crypto_verify16(computed_mac, mac);

        // Step 4: Only decrypt if MAC is valid
        if (valid == 0) {
            crypto_chacha20_ietf(plain_text, cipher_text, text_size, key, nonce, 1);
        }

        // Clean up
        crypto_wipe(poly_key, sizeof(poly_key));
        crypto_wipe(mac_data, total_mac_data_size);
        free(mac_data);

        return valid; // 0 = success, -1 = invalid MAC
    }

#ifdef __cplusplus
    }
#endif



