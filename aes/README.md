# AES Encryption Tool

A from-scratch implementation of the AES (Rijndael) cipher in C, with an interactive command-line interface for encrypting and decrypting messages. Supports all three standard key sizes.

## Overview

This implements AES-128, AES-192, and AES-256 following FIPS 197: SubBytes, ShiftRows, MixColumns, AddRoundKey and their inverses, plus the key schedule expansion. Round counts are 10, 12, and 14 respectively.

## Requirements

- GCC or another C99-compatible compiler
- make
- Linux, macOS, or Windows with MinGW

## Project structure

```
aes/
├── include/
│   └── aes.h          Type definitions and function prototypes
├── src/
│   ├── aes.c          AES algorithm implementation
│   └── main.c         Interactive CLI
├── test/
│   └── test.c         Standalone test file (not wired into the Makefile)
└── makefile
```

## Building

```bash
make          # build to build/aes_tool
make run      # build and run
make clean    # remove build artifacts
```

Manual build, if you'd rather skip the Makefile:

```bash
mkdir -p build
gcc -Wall -Wextra -g -std=c99 -o build/aes_tool src/main.c src/aes.c -I./include
```

## Usage

Run `make run` or `./build/aes_tool`, then follow the prompts:

1. Choose a key size (AES-128, AES-192, or AES-256).
2. Enter a key of the matching length (16, 24, or 32 characters).
3. Choose encrypt or decrypt.
4. Enter the message (for encryption) or ciphertext (for decryption).

Example session:

```
Select key size:
  1. AES-128 (16 bytes key)
  2. AES-192 (24 bytes key)
  3. AES-256 (32 bytes key)
  4. Exit

Choice: 1
AES-128 selected (16-byte key)

Enter encryption key (16 characters): MySecretKey12345
Key: 4D 79 53 65 63 72 65 74 4B 65 79 31 32 33 34 35

Select operation:
  1. Encrypt message
  2. Decrypt message
  3. Back to main menu

Choice: 1

Enter message to encrypt: Hello World!
Original: "Hello World!"
Length: 12 bytes
Padded length: 16 bytes
Plaintext (with padding): 48 65 6C 6C 6F 20 57 6F 72 6C 64 21 04 04 04 04

Ciphertext: A3 F2 8B 9C 1D 4E 7A 2F 8C 3B 6E 1A 9F 4D 2C 7E
```

## Technical details

Encryption: initial AddRoundKey, then N-1 rounds of SubBytes/ShiftRows/MixColumns/AddRoundKey, then a final round without MixColumns. Decryption mirrors this with the inverse operations, applied in reverse order.

Padding follows PKCS#7: a message short of a 16-byte boundary gets padded with bytes whose value equals the padding length (4 bytes needed becomes `04 04 04 04`), and a full block of padding is added even if the message is already block-aligned.

Galois field multiplication (used by MixColumns) uses the irreducible polynomial `x^8 + x^4 + x^3 + x + 1` (0x11B), implemented with the peasant's algorithm and modular reduction.

## API reference

```c
aes_error_t aes_encrypt(const uint8_t *plaintext, uint8_t *ciphertext,
                        const uint8_t *key, aes_key_size_t key_size);

aes_error_t aes_decrypt(const uint8_t *ciphertext, uint8_t *plaintext,
                        const uint8_t *key, aes_key_size_t key_size);

void expand_key(uint8_t *expanded_key, const uint8_t *key,
                aes_key_size_t key_size, size_t expanded_key_size);

const char *aes_error_to_string(aes_error_t error_code);
```

```c
typedef enum {
    AES_KEY_128 = 16,
    AES_KEY_192 = 24,
    AES_KEY_256 = 32
} aes_key_size_t;

typedef enum {
    AES_SUCCESS = 0,
    AES_MEMORY_ALLOCATION_FAILED,
    AES_ERROR_UNSUPORTED_KEY_SIZE
} aes_error_t;

typedef uint8_t aes_matrix_state_t[4][4];
```

## Security considerations

This is an educational project, not a cryptography library. It has no cipher mode (so encryption is effectively ECB), no IV, no message authentication, and no constant-time guarantees; keys sit in plaintext memory with no secure wiping. Don't use it to protect real data. For that, reach for an audited library like OpenSSL, libsodium, or mbedTLS, and use an authenticated mode such as AES-GCM.

## Testing

`make run` walks through an interactive round trip. For a scripted check against a known vector (FIPS 197):

```
Key:        2b7e151628aed2a6abf7158809cf4f3c
Plaintext:  3243f6a8885a308d313198a2e0370734
Ciphertext: 3925841d02dc09fbdc118597196a0b32
```

Build, run, select AES-128, use the key above, encrypt the plaintext, and compare against the expected ciphertext.

## References

- [FIPS 197 - AES Specification](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf)
- [The Rijndael Block Cipher - AES Proposal](https://csrc.nist.gov/csrc/media/projects/cryptographic-standards-and-guidelines/documents/aes-development/rijndael-ammended.pdf)
- [Wikipedia - Advanced Encryption Standard](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)

## License

MIT. See the repository root for licensing notes across projects.
