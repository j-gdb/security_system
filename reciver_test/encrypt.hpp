#ifndef CSC385_ENCRYPT
#define CSC385_ENCRYPT
#include "mbed.h"
#include "mbedtls/gcm.h"
#include "mbedtls/base64.h"


#define AES_KEY_SIZE     16   // 16 bytes = AES-128
#define GCM_IV_SIZE      12   // Standard IV size for GCM
#define GCM_TAG_SIZE     16   // Authentication tag size

const unsigned char key[AES_KEY_SIZE] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const unsigned char iv[GCM_IV_SIZE] = { 1,2,3,4,5,6,7,8,9,10,11,12 };

/*
 * @brief Uses and aes to encrypt and decrypt the  
 */
int encrypt_data(const unsigned char *key, 
                    const unsigned char *plaintext, size_t length,
                    unsigned char *b64_output, size_t b64_size, size_t *b64_len);

int decrypt_data(const unsigned char *key,
                    const unsigned char *b64_input, size_t b64_len,
                    unsigned char *plaintext, size_t *plaintext_len);

#endif //CSC385_ENCRYPT