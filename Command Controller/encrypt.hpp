#ifndef CSC385_ENCRYPT
#define CSC385_ENCRYPT
#include "mbed.h"
#include "mbedtls/gcm.h"
#include "mbedtls/base64.h"


#define AES_KEY_SIZE 16
#define GCM_IV_SIZE 12 //chars are 8 bits
#define GCM_TAG_SIZE 16

// fixed key
const unsigned char key[AES_KEY_SIZE] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
// the initialization vector is fixed because when I tried to randomly generate one I would get a hard fault no matter what
const unsigned char iv[GCM_IV_SIZE] = { '1','2','3','4','5','6','7','8','9','0','A','B' };

/*
 * @brief Encrypts the data using AES GCM
 * key: The AES key
 * plaintext: The text to encrypt
 * length: The length of the text
 * b64_output: The encyrpted text in base64. This is set by the function
 * b64_size: The size of the b64_output array. This is set by the function
 * b64_len: The number of bytes actually encoded. This is set by the function
 */
int encrypt_data(const unsigned char *key, const unsigned char *plaintext, size_t length, 
                        unsigned char *b64_output, size_t b64_size, size_t *b64_len);

/*
 * @brief Decrypts the data that was encrypted using AES GCM
 * b64_input: The encrypted data in base64
 * b64_len
 * length: The length of the text
 * b64_output: The encyrpted text in base64. This is set by the function
 * b64_size: The size of the b64_output array. This is set by the function
 * b64_len: The number of bytes actually encoded. This is set by the function
 */
int decrypt_data(const unsigned char *key, const unsigned char *b64_input, size_t b64_len, 
                                            unsigned char *plaintext, size_t *plaintext_len);

#endif //CSC385_ENCRYPT