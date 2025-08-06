#include "encrypt.hpp"
#include <stdio.h>
#include <string.h>

// Encrypt: key + plaintext -> Base64([IV|Ciphertext|Tag]

// Decrypt: key + Base64([IV|Ciphertext|Tag]) -> plaintext


// === Example usage ===
int main(void) {

    unsigned char plaintext[] = "Hello from encrypt!";
    unsigned char b64_output[512];
    size_t b64_len;

    // Encrypt
    if (encrypt_data(key, plaintext, strlen((char*)plaintext),
                        b64_output, sizeof(b64_output), &b64_len) == 0) {
        printf("Encrypted (Base64): %.*s\n", (int)b64_len, b64_output);
    } else {
        printf("Encryption failed.\n");
        return -1;
    }

    // Decrypt
    unsigned char decrypted[256];
    size_t decrypted_len;
    if (decrypt_data(key, b64_output, b64_len,
                        decrypted, &decrypted_len) == 0) {
        printf("Decrypted: %s\n", decrypted);
    } else {
        printf("Decryption failed or tag mismatch.\n");
    }

    return 0;
}