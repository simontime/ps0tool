#include "crc.h"

uint32_t crc_calculate(uint8_t *data, size_t length)
{
    uint32_t crc = ~0;
    
    while (length--)
        crc = crc_table[(crc & 0xff) ^ *data++] ^ (crc >> 8);
    
    return ~crc;
}
