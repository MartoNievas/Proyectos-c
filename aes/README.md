# AES Encryption Tool 🔐

A complete implementation of the AES (Advanced Encryption Standard) cipher algorithm in C, featuring an interactive command-line interface for message encryption and decryption.

## 📋 Overview

This project implements the AES (Rijndael) algorithm from scratch, supporting all three standard cipher variants:

- **AES-128**: 128-bit key (16 bytes)
- **AES-192**: 192-bit key (24 bytes)  
- **AES-256**: 256-bit key (32 bytes)

The implementation includes all fundamental AES operations: SubBytes, ShiftRows, MixColumns, AddRoundKey and their inverses, along with the Key Schedule expansion algorithm.

## ✨ Features

- ✅ Full implementation of AES-128, AES-192, and AES-256
- ✅ Interactive command-line interface
- ✅ Automatic PKCS#7 padding
- ✅ Variable-length message support
- ✅ Hexadecimal data visualization
- ✅ Block-based encryption/decryption (128-bit blocks)
- ✅ Robust error handling

## 🛠️ Requirements

- GCC compiler (or C99-compatible compiler)
- Make utility
- Operating System: Linux, macOS, or Windows with MinGW

## 📦 Project Structure

```
aes-encryption-tool/
│
├── include/
│   └── aes.h              # Type definitions and function prototypes
│
├── src/
│   ├── aes.c              # AES algorithm implementation
│   └── main.c             # Interactive main program
│
├── Makefile               # Build automation script
└── README.md              # This file
```

## 🚀 Building

### Quick build:

```bash
make
```

### Available targets:

```bash
make all      # Compile the project (default)
make clean    # Remove build artifacts
make run      # Compile and execute the program
```

### Build system details:

The Makefile uses the following configuration:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
INCLUDE = -I./include
BUILD_DIR = build
TARGET = $(BUILD_DIR)/aes_tool
```

**Compiler flags:**
- `-Wall -Wextra`: Enable all warnings
- `-g`: Include debugging symbols
- `-std=c99`: Use C99 standard
- `-I./include`: Specify header file location

**Build process:**
1. Creates `build/` directory if it doesn't exist
2. Compiles source files to object files
3. Links object files into final executable
4. Outputs binary to `build/aes_tool`

### Manual compilation (if needed):

```bash
mkdir -p build
gcc -Wall -Wextra -g -std=c99 -o build/aes_tool src/main.c src/aes.c -I./include
```

## 💻 Usage

### Running the program:

```bash
make run
# or
./build/aes_tool
```

### Workflow:

1. **Select key size:**
   ```
   1. AES-128 (16 bytes key)
   2. AES-192 (24 bytes key)
   3. AES-256 (32 bytes key)
   ```

2. **Enter encryption key:**
   - AES-128: exactly 16 characters
   - AES-192: exactly 24 characters
   - AES-256: exactly 32 characters

3. **Select operation:**
   - Encrypt message
   - Decrypt message

4. **Input message** (for encryption) or use previous ciphertext (for decryption)

### Example session:

```
╔════════════════════════════════════════╗
║     AES Encryption/Decryption Tool    ║
╚════════════════════════════════════════╝

Select key size:
  1. AES-128 (16 bytes key)
  2. AES-192 (24 bytes key)
  3. AES-256 (32 bytes key)
  4. Exit

Choice: 1

✓ AES-128 selected (16-byte key)

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

🔒 Encrypting...
✓ Encryption successful!

Ciphertext: A3 F2 8B 9C 1D 4E 7A 2F 8C 3B 6E 1A 9F 4D 2C 7E
```

## 🔧 Technical Details

### AES Algorithm Implementation

The implementation follows the FIPS 197 specification with these transformations:

**Encryption rounds:**
1. Initial AddRoundKey (using original key)
2. For rounds 1 to N-1:
   - SubBytes (S-box substitution)
   - ShiftRows (cyclic row shifts)
   - MixColumns (column mixing in GF(2^8))
   - AddRoundKey (XOR with round key)
3. Final round (N):
   - SubBytes
   - ShiftRows
   - AddRoundKey (no MixColumns)

**Decryption rounds:**
1. Initial AddRoundKey (using last round key)
2. For rounds N-1 to 1:
   - InvShiftRows
   - InvSubBytes
   - AddRoundKey
   - InvMixColumns
3. Final round:
   - InvShiftRows
   - InvSubBytes
   - AddRoundKey (no InvMixColumns)

**Number of rounds:**
- AES-128: 10 rounds
- AES-192: 12 rounds
- AES-256: 14 rounds

### PKCS#7 Padding Scheme

Messages not aligned to 16-byte blocks are padded using PKCS#7:
- Padding byte value equals the number of bytes added
- Example: If 4 bytes needed → append `04 04 04 04`
- Always applied, even if message is block-aligned (adds full block)

### Lookup Tables

- **S-Box (256 bytes)**: Substitution table for SubBytes transformation
- **Inverse S-Box (256 bytes)**: Inverse substitution for InvSubBytes
- **RCON (32 bytes)**: Round constants for key schedule

### Galois Field Arithmetic

Implements multiplication in GF(2^8) using the irreducible polynomial:
```
m(x) = x^8 + x^4 + x^3 + x + 1 (0x11B in hex)
```

The `galois_mul()` function performs multiplication via the peasant's algorithm with modular reduction.

## 📊 API Reference

### Core Functions

```c
/**
 * Encrypts a 16-byte block using AES
 * @param plaintext  Input block (16 bytes)
 * @param ciphertext Output block (16 bytes)
 * @param key        Encryption key
 * @param key_size   Key size (AES_KEY_128/192/256)
 * @return Error code
 */
aes_error_t aes_encrypt(
    const uint8_t *plaintext,
    uint8_t *ciphertext,
    const uint8_t *key,
    aes_key_size_t key_size
);

/**
 * Decrypts a 16-byte block using AES
 * @param ciphertext Input block (16 bytes)
 * @param plaintext  Output block (16 bytes)
 * @param key        Decryption key
 * @param key_size   Key size (AES_KEY_128/192/256)
 * @return Error code
 */
aes_error_t aes_decrypt(
    const uint8_t *ciphertext,
    uint8_t *plaintext,
    const uint8_t *key,
    aes_key_size_t key_size
);

/**
 * Expands the cipher key into round keys
 * @param expanded_key     Output buffer for expanded key
 * @param key              Input cipher key
 * @param key_size         Size of input key
 * @param expanded_key_size Size of output buffer
 */
void expand_key(
    uint8_t *expanded_key,
    const uint8_t *key,
    aes_key_size_t key_size,
    size_t expanded_key_size
);

/**
 * Converts error code to string description
 * @param error_code Error code enum
 * @return Error description string
 */
const char *aes_error_to_string(aes_error_t error_code);
```

### Type Definitions

```c
/**
 * Supported AES key sizes
 */
typedef enum {
    AES_KEY_128 = 16,  // 128-bit key
    AES_KEY_192 = 24,  // 192-bit key
    AES_KEY_256 = 32   // 256-bit key
} aes_key_size_t;

/**
 * Error codes returned by AES functions
 */
typedef enum {
    AES_SUCCESS = 0,                    // Operation successful
    AES_MEMORY_ALLOCATION_FAILED,       // malloc() failed
    AES_ERROR_UNSUPORTED_KEY_SIZE       // Invalid key size
} aes_error_t;

/**
 * AES state matrix (4x4 bytes)
 */
typedef uint8_t aes_matrix_state_t[4][4];
```

### Constants

```c
#define AES_BLOCK_SIZE 16          // Block size in bytes
#define STATE_MATRIX_SIZE 4        // State matrix dimension
#define WORD_SIZE 4                // Word size in bytes
#define BITS_PER_BYTE 8            // Bits per byte
#define GF_REDUCING_POLYNOMIAL 0x1B // Galois field polynomial
#define GF_MSB_MASK 0x80           // MSB mask for GF operations
```

## ⚠️ Security Considerations

**THIS IS AN EDUCATIONAL PROJECT.** For production use, consider:

### Known limitations:
- ❌ No cipher mode implementation (effectively ECB)
- ❌ No message authentication (MAC/AEAD)
- ❌ No IV (Initialization Vector) support
- ❌ Keys stored in plaintext in memory
- ❌ Not hardened against side-channel attacks
- ❌ No constant-time operations
- ❌ No secure memory wiping

### Production recommendations:
- ✅ Use audited cryptographic libraries (OpenSSL, libsodium, mbedTLS)
- ✅ Implement secure modes of operation (CBC, CTR, GCM)
- ✅ Add authenticated encryption (AES-GCM, ChaCha20-Poly1305)
- ✅ Use random IVs for each encryption
- ✅ Implement secure key derivation (PBKDF2, Argon2)
- ✅ Clear sensitive data from memory after use
- ✅ Use constant-time operations to prevent timing attacks

**DO NOT USE THIS CODE FOR:**
- Production applications requiring security
- Protecting sensitive data
- Any system where security is critical

## 🧪 Testing

### Basic functionality test:

```bash
make run
```

### Test vectors (from FIPS 197):

**AES-128 Test:**
```
Key:        2b7e151628aed2a6abf7158809cf4f3c
Plaintext:  3243f6a8885a308d313198a2e0370734
Ciphertext: 3925841d02dc09fbdc118597196a0b32
```

### Verification steps:

1. Compile the program: `make`
2. Run: `./build/aes_tool`
3. Select AES-128
4. Enter a 16-character key
5. Encrypt a test message
6. Decrypt the ciphertext
7. Verify original message is recovered

### Unit testing (future work):

```c
// Example test structure
void test_aes_128_encryption() {
    uint8_t key[16] = {0x2b, 0x7e, 0x15, ...};
    uint8_t plaintext[16] = {0x32, 0x43, 0xf6, ...};
    uint8_t expected[16] = {0x39, 0x25, 0x84, ...};
    uint8_t ciphertext[16];
    
    aes_encrypt(plaintext, ciphertext, key, AES_KEY_128);
    assert(memcmp(ciphertext, expected, 16) == 0);
}
```

## 📚 References

### Standards and specifications:
- [FIPS 197 - AES Specification](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf)
- [The Rijndael Block Cipher - AES Proposal](https://csrc.nist.gov/csrc/media/projects/cryptographic-standards-and-guidelines/documents/aes-development/rijndael-ammended.pdf)

### Educational resources:
- [Wikipedia - Advanced Encryption Standard](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)
- [A Stick Figure Guide to AES](http://www.moserware.com/2009/09/stick-figure-guide-to-advanced.html)
- [AES Animation](https://www.youtube.com/watch?v=mlzxpkdXP58)

### Cryptographic best practices:
- [OWASP Cryptographic Storage Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Cryptographic_Storage_Cheat_Sheet.html)
- [libsodium Documentation](https://doc.libsodium.org/)

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/YourFeature`
3. Commit changes: `git commit -m 'Add YourFeature'`
4. Push to branch: `git push origin feature/YourFeature`
5. Submit a Pull Request

### Code style:
- Follow K&R indentation style
- Use descriptive variable names
- Comment complex algorithms
- Maintain consistent formatting

### Testing:
- Test all three key sizes (128/192/256)
- Verify encrypt/decrypt roundtrip
- Check error handling paths
- Test edge cases (empty input, max length, etc.)

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👨‍💻 Author

Developed as an educational project to understand AES implementation details.

## 🙏 Acknowledgments

- NIST for the AES specification
- Joan Daemen and Vincent Rijmen for the Rijndael algorithm
- The open-source cryptography community

---

**⚠️ DISCLAIMER**: This is an educational implementation. Do not use in production systems without professional security audit. For real-world applications, use established cryptographic libraries.
