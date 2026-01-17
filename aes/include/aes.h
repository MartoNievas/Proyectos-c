#ifndef AES_H
#define AES_H

/*Defines aes errors*/
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

#define AES_ROUNDS_128 10
#define AES_ROUND_192 12
#define AES_ROUNDS_256 14

#define AES_MATRIX_SIZE 4

#define AEX_BLOCK_SIZE 16

#define WORD_SIZE 4
#define BITS_PER_BYTE 8

#endif // !AES_DEF
