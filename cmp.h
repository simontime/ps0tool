#pragma once

#include <stdint.h>

#define XOR_KEY 0x95

typedef struct
{
    int      bits_left;
    int      ctrl_byte;
    uint8_t *data;
} compression_ctx_t;

int uncompress(uint8_t *in, uint8_t *out);
