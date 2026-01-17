/*
 * @file aex.c
 * @brief An implementation of the AES (Rijndael) algorithm
 * */

#include "../include/aes.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*definition of Subtituin box*/

static const uint8_t sbox[256];
