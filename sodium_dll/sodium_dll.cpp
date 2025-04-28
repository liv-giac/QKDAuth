// dllmain.cpp : Definisce il punto di ingresso per l'applicazione DLL.
#include "pch.h"

// sodium_wrapper.c
#include <sodium.h>
#include <iostream>

__declspec(dllexport) int chacha20_encrypt(
    const unsigned char* message, unsigned long long message_len,
    const unsigned char* aad, unsigned long long aad_len,
    const unsigned char* nonce,
    const unsigned char* key,
    unsigned char* ciphertext,
    unsigned char* mac)
{
    if (sodium_init() < 0) return -1;

    unsigned char* temp = new unsigned char[message_len + crypto_aead_chacha20poly1305_ietf_ABYTES];
    unsigned long long clen = 0;
    memset(temp, 0, message_len + crypto_aead_chacha20poly1305_ietf_ABYTES);

    int result = crypto_aead_chacha20poly1305_ietf_encrypt(
        temp, &clen,
        message, message_len,
        aad, aad_len,
        NULL, nonce, key
    );

    if (result != 0 || clen != message_len + crypto_aead_chacha20poly1305_ietf_ABYTES) {
        delete[] temp;
        return -1;
    }

    // Debug output
    std::cout<<"DLL Encrypt: clen ="<<clen<<std::endl;
    std::cout<<"DLL Encrypt: MAC = ";
    for (int i = 0; i < crypto_aead_chacha20poly1305_ietf_ABYTES; i++) {
        std::cout<<temp[message_len + i];
    }
	std::cout << std::endl;
    memcpy(ciphertext, temp, message_len);
    memcpy(mac, temp + message_len, crypto_aead_chacha20poly1305_ietf_ABYTES);

    delete[] temp;
    return 0;
}
__declspec(dllexport) int chacha20_decrypt(
    const unsigned char* ciphertext, unsigned long long message_len,
    const unsigned char* aad, unsigned long long aad_len,
    const unsigned char* mac,
    const unsigned char* nonce,
    const unsigned char* key,
    unsigned char* decrypted)
{
    if (sodium_init() < 0) return -1;

    unsigned char* combined = new unsigned char[message_len + crypto_aead_chacha20poly1305_ietf_ABYTES];
    memcpy(combined, ciphertext, message_len);
    memcpy(combined + message_len, mac, crypto_aead_chacha20poly1305_ietf_ABYTES);

    int result = crypto_aead_chacha20poly1305_ietf_decrypt(
        decrypted, NULL,
        NULL,
        combined, message_len + crypto_aead_chacha20poly1305_ietf_ABYTES,
        aad, aad_len,
        nonce, key
    );

    delete[] combined;
    return result;
}

