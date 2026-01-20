# AES Encryption Tool 🔐

Una implementación completa del algoritmo de cifrado AES (Advanced Encryption Standard) en C, con una interfaz interactiva de línea de comandos para encriptar y desencriptar mensajes.

## 📋 Descripción

Este proyecto implementa el algoritmo AES (Rijndael) desde cero, soportando las tres variantes estándar del cifrado:

- **AES-128**: Clave de 128 bits (16 bytes)
- **AES-192**: Clave de 192 bits (24 bytes)  
- **AES-256**: Clave de 256 bits (32 bytes)

La implementación incluye todas las operaciones fundamentales de AES: SubBytes, ShiftRows, MixColumns, AddRoundKey y sus inversas, junto con la expansión de claves (Key Schedule).

## ✨ Características

- ✅ Implementación completa de AES-128, AES-192 y AES-256
- ✅ Interfaz interactiva de usuario
- ✅ Padding PKCS#7 automático
- ✅ Soporte para mensajes de longitud variable
- ✅ Visualización hexadecimal de datos
- ✅ Encriptación y desencriptación en bloques de 128 bits
- ✅ Manejo de errores robusto

## 🛠️ Requisitos

- Compilador GCC (o compatible con C99)
- Make (opcional)
- Sistema operativo: Linux, macOS, o Windows con MinGW

## 📦 Estructura del Proyecto

```
aes-encryption-tool/
│
├── include/
│   └── aes.h              # Definiciones y prototipos
│
├── src/
│   ├── aes.c              # Implementación del algoritmo AES
│   └── main.c             # Programa principal interactivo
│
├── README.md              # Este archivo
└── Makefile               # Script de compilación (opcional)
```

## 🚀 Compilación

### Compilación manual:

```bash
gcc -o aes_tool src/main.c src/aes.c -I./include -Wall -Wextra -std=c99
```

### Con Makefile (si está disponible):

```bash
make
```

## 💻 Uso

### Ejecución del programa:

```bash
./aes_tool
```

### Flujo de trabajo:

1. **Seleccionar el tamaño de clave:**
   ```
   1. AES-128 (16 bytes key)
   2. AES-192 (24 bytes key)
   3. AES-256 (32 bytes key)
   ```

2. **Ingresar la clave de encriptación:**
   - Para AES-128: exactamente 16 caracteres
   - Para AES-192: exactamente 24 caracteres
   - Para AES-256: exactamente 32 caracteres

3. **Seleccionar operación:**
   - Encriptar mensaje
   - Desencriptar mensaje

4. **Ingresar el mensaje** (para encriptación) o usar el último mensaje encriptado (para desencriptación)

### Ejemplo de sesión:

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

Select operation:
  1. Encrypt message
  2. Decrypt message
  3. Back to main menu

Choice: 2

Using previous ciphertext for decryption...

🔓 Decrypting...
✓ Decryption successful!

Decrypted (with padding): 48 65 6C 6C 6F 20 57 6F 72 6C 64 21 04 04 04 04
Decrypted (no padding): 48 65 6C 6C 6F 20 57 6F 72 6C 64 21
Decrypted message: "Hello World!"
```

## 🔧 Detalles Técnicos

### Algoritmo AES

El algoritmo implementa las siguientes transformaciones:

**Encriptación:**
1. AddRoundKey (clave inicial)
2. Para cada ronda (excepto la última):
   - SubBytes
   - ShiftRows
   - MixColumns
   - AddRoundKey
3. Ronda final:
   - SubBytes
   - ShiftRows
   - AddRoundKey

**Desencriptación:**
1. AddRoundKey (última clave)
2. Para cada ronda (excepto la última):
   - InvShiftRows
   - InvSubBytes
   - AddRoundKey
   - InvMixColumns
3. Ronda final:
   - InvShiftRows
   - InvSubBytes
   - AddRoundKey

### Padding PKCS#7

Los mensajes que no son múltiplos de 16 bytes se rellenan usando el estándar PKCS#7:
- El valor del byte de padding es igual al número de bytes agregados
- Ejemplo: Si faltan 4 bytes, se agregan `04 04 04 04`

### S-Box y Tablas

- **S-Box**: Tabla de sustitución de 256 valores para SubBytes
- **Inverse S-Box**: Tabla inversa para InvSubBytes
- **RCON**: Constantes de ronda para la expansión de claves

### Multiplicación en GF(2^8)

El programa implementa la multiplicación en el campo de Galois GF(2^8) con el polinomio irreducible:
```
x^8 + x^4 + x^3 + x + 1 (0x11B)
```

## 📊 API del Código

### Funciones principales:

```c
// Encriptar un bloque de 16 bytes
aes_error_t aes_encrypt(
    const uint8_t *plaintext,
    uint8_t *ciphertext,
    const uint8_t *key,
    aes_key_size_t key_size
);

// Desencriptar un bloque de 16 bytes
aes_error_t aes_decrypt(
    const uint8_t *ciphertext,
    uint8_t *plaintext,
    const uint8_t *key,
    aes_key_size_t key_size
);

// Expandir la clave
void expand_key(
    uint8_t *expanded_key,
    const uint8_t *key,
    aes_key_size_t key_size,
    size_t expanded_key_size
);

// Convertir código de error a string
const char *aes_error_to_string(aes_error_t error_code);
```

### Tipos de datos:

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

## ⚠️ Consideraciones de Seguridad

**Este proyecto es para fines educativos.** Para uso en producción, considere:

- ✅ Usar bibliotecas criptográficas auditadas (OpenSSL, libsodium)
- ✅ Implementar modos de operación seguros (CBC, GCM)
- ✅ Agregar vectores de inicialización (IV)
- ✅ Implementar autenticación de mensajes (MAC)
- ✅ Proteger claves en memoria
- ✅ Usar generadores de números aleatorios criptográficamente seguros

**Vulnerabilidades conocidas en esta implementación:**
- No usa modo de operación (solo ECB implícito)
- No tiene autenticación de mensajes
- Las claves se manejan en texto plano en memoria
- No protege contra ataques de canal lateral (timing attacks)

## 🧪 Testing

Para verificar que la implementación funciona correctamente:

```bash
# Compilar
gcc -o aes_tool src/main.c src/aes.c -I./include -Wall -Wextra

# Ejecutar
./aes_tool

# Probar con vectores de test conocidos:
# Mensaje: "TestMessage12345"
# Clave AES-128: "0123456789ABCDEF"
# Verificar que encriptar y desencriptar devuelve el mensaje original
```

## 📚 Referencias

- [FIPS 197 - AES Specification](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf)
- [The Rijndael Block Cipher](https://csrc.nist.gov/csrc/media/projects/cryptographic-standards-and-guidelines/documents/aes-development/rijndael-ammended.pdf)
- [Wikipedia - Advanced Encryption Standard](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Por favor:

1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## 📝 Licencia

Este proyecto está bajo la Licencia MIT. Ver el archivo `LICENSE` para más detalles.

## 👨‍💻 Autor

Desarrollado como proyecto educativo para entender la implementación del algoritmo AES.

## 🙏 Agradecimientos

- Al NIST por la especificación AES
- A Joan Daemen y Vincent Rijmen por el algoritmo Rijndael
- A la comunidad de criptografía de código abierto

---

**Nota**: Este es un proyecto educativo. No use este código para aplicaciones que requieran seguridad real sin una auditoría de seguridad profesional.
