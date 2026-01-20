/*
 * @file main.c
 * @brief AES Encryption/Decryption Test Program
 */

#include "../include/aes.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

// Función para imprimir bytes en formato hexadecimal
void print_hex(const char *label, const uint8_t *data, size_t len) {
  printf("%s: ", label);
  for (size_t i = 0; i < len; i++) {
    printf("%02X ", data[i]);
    if ((i + 1) % 16 == 0 && i + 1 < len)
      printf("\n%*s", (int)strlen(label) + 2, "");
  }
  printf("\n");
}

// Función para imprimir como string (si es imprimible)
void print_string(const char *label, const uint8_t *data, size_t len) {
  printf("%s: \"", label);
  for (size_t i = 0; i < len; i++) {
    if (data[i] >= 32 && data[i] <= 126)
      printf("%c", data[i]);
    else
      printf(".");
  }
  printf("\"\n");
}

// Aplicar padding PKCS#7
size_t apply_padding(uint8_t *data, size_t len, size_t max_len) {
  size_t padding_len = AES_BLOCK_SIZE - (len % AES_BLOCK_SIZE);
  if (len + padding_len > max_len) {
    return 0; // No hay espacio para padding
  }

  for (size_t i = 0; i < padding_len; i++) {
    data[len + i] = (uint8_t)padding_len;
  }

  return len + padding_len;
}

// Remover padding PKCS#7
size_t remove_padding(uint8_t *data, size_t len) {
  if (len == 0 || len % AES_BLOCK_SIZE != 0) {
    return len;
  }

  uint8_t padding_len = data[len - 1];

  // Validar padding
  if (padding_len == 0 || padding_len > AES_BLOCK_SIZE) {
    return len;
  }

  for (size_t i = len - padding_len; i < len; i++) {
    if (data[i] != padding_len) {
      return len; // Padding inválido
    }
  }

  return len - padding_len;
}

void print_menu() {
  printf("\n");
  printf("╔════════════════════════════════════════╗\n");
  printf("║     AES Encryption/Decryption Tool    ║\n");
  printf("╚════════════════════════════════════════╝\n");
  printf("\n");
  printf("Select key size:\n");
  printf("  1. AES-128 (16 bytes key)\n");
  printf("  2. AES-192 (24 bytes key)\n");
  printf("  3. AES-256 (32 bytes key)\n");
  printf("  4. Exit\n");
  printf("\n");
  printf("Choice: ");
}

void print_operation_menu() {
  printf("\nSelect operation:\n");
  printf("  1. Encrypt message\n");
  printf("  2. Decrypt message\n");
  printf("  3. Back to main menu\n");
  printf("\nChoice: ");
}

int main(void) {
  int choice, operation;
  char input[MAX_INPUT_SIZE];
  uint8_t key[32]; // Tamaño máximo para AES-256
  uint8_t plaintext[MAX_INPUT_SIZE];
  uint8_t ciphertext[MAX_INPUT_SIZE];
  uint8_t decrypted[MAX_INPUT_SIZE];
  aes_key_size_t key_size;
  size_t key_len;

  printf("╔════════════════════════════════════════════════════════╗\n");
  printf("║                                                        ║\n");
  printf("║          Welcome to AES Encryption Tool               ║\n");
  printf("║                                                        ║\n");
  printf("╚════════════════════════════════════════════════════════╝\n");

  while (1) {
    print_menu();

    if (scanf("%d", &choice) != 1) {
      while (getchar() != '\n')
        ; // Limpiar buffer
      printf("\n⚠ Invalid input. Please enter a number.\n");
      continue;
    }
    while (getchar() != '\n')
      ; // Limpiar buffer

    if (choice == 4) {
      printf("\n👋 Goodbye!\n\n");
      break;
    }

    // Determinar tamaño de clave
    switch (choice) {
    case 1:
      key_size = AES_KEY_SIZE_128;
      key_len = 16;
      printf("\n✓ AES-128 selected (16-byte key)\n");
      break;
    case 2:
      key_size = AES_KEY_SIZE_192;
      key_len = 24;
      printf("\n✓ AES-192 selected (24-byte key)\n");
      break;
    case 3:
      key_size = AES_KEY_SIZE_256;
      key_len = 32;
      printf("\n✓ AES-256 selected (32-byte key)\n");
      break;
    default:
      printf("\n⚠ Invalid choice. Please try again.\n");
      continue;
    }

    // Obtener la clave
    printf("\nEnter encryption key (%zu characters): ", key_len);
    if (fgets(input, sizeof(input), stdin) == NULL) {
      printf("\n⚠ Error reading key.\n");
      continue;
    }

    // Remover el newline
    input[strcspn(input, "\n")] = 0;

    if (strlen(input) != key_len) {
      printf("\n⚠ Key must be exactly %zu characters. Got %zu characters.\n",
             key_len, strlen(input));
      continue;
    }

    memcpy(key, input, key_len);
    print_hex("Key", key, key_len);

    // Menú de operaciones
    while (1) {
      print_operation_menu();

      if (scanf("%d", &operation) != 1) {
        while (getchar() != '\n')
          ;
        printf("\n⚠ Invalid input.\n");
        continue;
      }
      while (getchar() != '\n')
        ;

      if (operation == 3) {
        break; // Volver al menú principal
      }

      if (operation == 1) {
        // ENCRIPTAR
        printf("\nEnter message to encrypt: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
          printf("\n⚠ Error reading message.\n");
          continue;
        }

        input[strcspn(input, "\n")] = 0;
        size_t msg_len = strlen(input);

        if (msg_len == 0) {
          printf("\n⚠ Message cannot be empty.\n");
          continue;
        }

        memcpy(plaintext, input, msg_len);

        // Aplicar padding
        size_t padded_len = apply_padding(plaintext, msg_len, MAX_INPUT_SIZE);
        if (padded_len == 0) {
          printf("\n⚠ Message too long.\n");
          continue;
        }

        printf("\n");
        print_string("Original", plaintext, msg_len);
        printf("Length: %zu bytes\n", msg_len);
        printf("Padded length: %zu bytes\n", padded_len);
        print_hex("Plaintext (with padding)", plaintext, padded_len);

        // Encriptar bloque por bloque
        printf("\n🔒 Encrypting...\n");
        for (size_t i = 0; i < padded_len; i += AES_BLOCK_SIZE) {
          aes_error_t err =
              aes_encrypt(plaintext + i, ciphertext + i, key, key_size);

          if (err != AES_SUCCESS) {
            printf("\n⚠ Encryption error: %s\n", aes_error_to_string(err));
            goto operation_end;
          }
        }

        printf("✓ Encryption successful!\n\n");
        print_hex("Ciphertext", ciphertext, padded_len);

      } else if (operation == 2) {
        // DESENCRIPTAR
        printf("\nUsing previous ciphertext for decryption...\n");

        if (ciphertext[0] == 0 && ciphertext[1] == 0) {
          printf(
              "\n⚠ No ciphertext available. Please encrypt a message first.\n");
          continue;
        }

        size_t cipher_len =
            ((strlen(input) / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;

        // Desencriptar bloque por bloque
        printf("\n🔓 Decrypting...\n");
        for (size_t i = 0; i < cipher_len; i += AES_BLOCK_SIZE) {
          aes_error_t err =
              aes_decrypt(ciphertext + i, decrypted + i, key, key_size);

          if (err != AES_SUCCESS) {
            printf("\n⚠ Decryption error: %s\n", aes_error_to_string(err));
            goto operation_end;
          }
        }

        // Remover padding
        size_t decrypted_len = remove_padding(decrypted, cipher_len);

        printf("✓ Decryption successful!\n\n");
        print_hex("Decrypted (with padding)", decrypted, cipher_len);
        print_hex("Decrypted (no padding)", decrypted, decrypted_len);
        print_string("Decrypted message", decrypted, decrypted_len);

      } else {
        printf("\n⚠ Invalid operation.\n");
      }

    operation_end:
      continue;
    }
  }

  return 0;
}
