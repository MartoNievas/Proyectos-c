#ifndef AES_H
#define AES_H

/*Defines aes errors*/
#include <stdint.h>
typedef enum {
  AES_SUCCES = 0,
  AES_ERROR_UNSUPORTED_KEY,
  AES_MEMORY_ALLOCATION_FAILED,
} aes_error_t;

/*Size of keys of keys in bytes */

typedef enum {
  AES_KEY_128 = 16,
  AES_KEY_192 = 24,
  AES_KEY_256 = 32,
} aes_key_size_t;

typedef enum {
  AES_WORD_128 = 4,
  AES_WORD_192 = 6,
  AES_WORD_256 = 8,
} aes_words_t;

/*Needed parameters for aes algorithm*/

typedef struct {
  aes_words_t keys;
  int rounds;
} aes_params_t;

#define AES_MAX_WORDS 60

typedef struct {
  uint8_t w[AES_MAX_WORDS][4];
  int Nk;
} rounds_keys_t;

#define STATE_MATRIX_SIZE 4

typedef uint8_t aes_matrix_state_t[STATE_MATRIX_SIZE][STATE_MATRIX_SIZE];
typedef uint8_t aes_round_key_t[STATE_MATRIX_SIZE][STATE_MATRIX_SIZE];

/*Public API*/

aes_error_t aes_encrypt();
#endif // !AES_DEF
