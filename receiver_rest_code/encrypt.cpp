#include "encrypt.hpp"

int encrypt_data(const unsigned char *key, 
                    const unsigned char *plaintext, size_t length,
                    unsigned char *b64_output, size_t b64_size, size_t *b64_len) 
{
    unsigned char iv[] = "1234567890AB"; // Fixed for now
    unsigned char ciphertext[256];
    unsigned char tag[GCM_TAG_SIZE];
    unsigned char combined[256 + GCM_IV_SIZE + GCM_TAG_SIZE];

    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);

    // Set AES key
    mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, AES_KEY_SIZE * 8);

    // Encrypt
    if (mbedtls_gcm_crypt_and_tag(&gcm, MBEDTLS_GCM_ENCRYPT,
                                  length,
                                  iv, GCM_IV_SIZE,
                                  NULL, 0,
                                  plaintext, ciphertext,
                                  GCM_TAG_SIZE, tag) != 0) {
        mbedtls_gcm_free(&gcm);
        return -1;
    }

    // Combine: IV | Ciphertext | Tag
    memcpy(combined, iv, GCM_IV_SIZE);
    memcpy(combined + GCM_IV_SIZE, ciphertext, length);
    memcpy(combined + GCM_IV_SIZE + length, tag, GCM_TAG_SIZE);

    // Encode to Base64
    mbedtls_base64_encode(b64_output, b64_size, b64_len,
                          combined, GCM_IV_SIZE + length + GCM_TAG_SIZE);

    mbedtls_gcm_free(&gcm);
    return 0;
}

int decrypt_data(const unsigned char *key,
                    const unsigned char *b64_input, size_t b64_len,
                    unsigned char *plaintext, size_t *plaintext_len)
{
    unsigned char decoded[256 + GCM_IV_SIZE + GCM_TAG_SIZE];
    size_t decoded_len;

    // Decode Base64
    mbedtls_base64_decode(decoded, sizeof(decoded), &decoded_len,
                          b64_input, b64_len);

    // Split IV, ciphertext, and tag
    unsigned char *iv = decoded;
    unsigned char *ciphertext = decoded + GCM_IV_SIZE;
    size_t ct_len = decoded_len - GCM_IV_SIZE - GCM_TAG_SIZE;
    unsigned char *tag = decoded + GCM_IV_SIZE + ct_len;

    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);

    mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, AES_KEY_SIZE * 8);

    // Decrypt with authentication
    int ret = mbedtls_gcm_auth_decrypt(&gcm, ct_len,
                                       iv, GCM_IV_SIZE,
                                       NULL, 0,
                                       tag, GCM_TAG_SIZE,
                                       ciphertext, plaintext);

    if (ret != 0) {
        mbedtls_gcm_free(&gcm);
        return -1; // Auth failed or decryption error
    }

    *plaintext_len = ct_len;
    plaintext[ct_len] = '\0'; // Null-terminate if text
    mbedtls_gcm_free(&gcm);
    return 0;
}