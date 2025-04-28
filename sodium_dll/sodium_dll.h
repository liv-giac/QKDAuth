
#include <sodium.h>

extern "C" {


    /**
     * Encrypts a message using ChaCha20-Poly1305 (IETF variant).
     *
     * @param message       Pointer to the plaintext message.
     * @param message_len   Length of the plaintext message.
     * @param aad           Pointer to additional authenticated data (can be NULL).
     * @param aad_len       Length of the additional authenticated data.
     * @param nonce         Pointer to the nonce (must be crypto_aead_chacha20poly1305_ietf_NPUBBYTES bytes).
     * @param key           Pointer to the encryption key (must be crypto_aead_chacha20poly1305_ietf_KEYBYTES bytes).
     * @param ciphertext    Pointer to the buffer where the ciphertext will be stored.
     * @param mac           Pointer to the buffer where the MAC will be stored (must be crypto_aead_chacha20poly1305_ietf_ABYTES bytes).
     *
     * @return 0 on success, -1 on failure.
     */
    __declspec(dllexport) int chacha20_encrypt(
        const unsigned char* message, unsigned long long message_len,
        const unsigned char* aad, unsigned long long aad_len,
        const unsigned char* nonce,
        const unsigned char* key,
        unsigned char* ciphertext,
        unsigned char* mac);

    /**
     * Decrypts a message using ChaCha20-Poly1305 (IETF variant).
     *
     * @param ciphertext        Pointer to the ciphertext.
     * @param ciphertext_len    Length of the ciphertext.
     * @param aad               Pointer to additional authenticated data (must be the same as used during encryption).
     * @param aad_len           Length of the additional authenticated data.
     * @param mac               Pointer to the MAC (must be crypto_aead_chacha20poly1305_ietf_ABYTES bytes).
     * @param nonce             Pointer to the nonce used during encryption.
     * @param key               Pointer to the decryption key (must be crypto_aead_chacha20poly1305_ietf_KEYBYTES bytes).
     * @param decrypted         Pointer to the buffer where the decrypted plaintext will be stored.
     *
     * @return 0 on success, -1 on failure.
     */
    __declspec(dllexport) int chacha20_decrypt(
        const unsigned char* ciphertext, unsigned long long ciphertext_len,
        const unsigned char* aad, unsigned long long aad_len,
        const unsigned char* mac,
        const unsigned char* nonce,
        const unsigned char* key,
        unsigned char* decrypted);


}

