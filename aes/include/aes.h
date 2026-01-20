#ifndef AES_H
#define AES_H

/*Defines aes errors*/
#include <stddef.h>
#include <stdint.h>
typedef enum {
  AES_SUCCESS = 0,
  AES_ERROR_UNSUPORTED_KEY_SIZE,
  AES_ERROR_MEMORY_ALLOCATION_FAILED,
} aes_error_t;

/*Size of keys of keys in bytes */

typedef enum {
  AES_KEY_SIZE_128 = 16,
  AES_KEY_SIZE_192 = 24,
  AES_KEY_SIZE_256 = 32,
} aes_key_size_t;

/*Galois Field (GF(2^3)) constan for the mix columns*/
#define GF_REDUCING_POLYNOMIAL 0x1B
/*Irreducible polynomial for AES: x^8 + x^4 + x^3 + x + 1*/
#define GF_MSB_MASK 0x80

#define AES_BLOCK_SIZE 16

#define STATE_MATRIX_SIZE 4

#define MAX_EXPANDED_KEY 240

typedef uint8_t aes_matrix_state_t[STATE_MATRIX_SIZE][STATE_MATRIX_SIZE];
typedef uint8_t aes_round_key_t[STATE_MATRIX_SIZE][STATE_MATRIX_SIZE];

/*Public API*/
void expand_key(uint8_t *expanded_key, const uint8_t *key,
                aes_key_size_t key_size, size_t expanded_key_size);

aes_error_t aes_encrypt(const uint8_t *plaintext, uint8_t *ciphertext,
                        const uint8_t *key, aes_key_size_t key_size);

aes_error_t aes_decrypt(const uint8_t *ciphertext, uint8_t *plaintext,
                        const uint8_t *key, aes_key_size_t key_size);

const char *aes_error_to_string(aes_error_t error_code);
#endif // !AES_DEF
