#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cmp.h"
#include "crc.h"
#include "util.h"

typedef struct
{
    char     magic[4];
    uint32_t crc;
    uint32_t size1;
    uint32_t size2;
} zpr_header_t;

int main(int argc, char **argv)
{
    FILE        *f_in, *f_out;
    uint8_t     *b_in, *b_out;
    int          s_in,  s_out;
    uint32_t     crc;
    zpr_header_t hdr;
    
    if (argc != 3)
    {
        printf("Usage: %s infile outfile\n", argv[0]);
        return 0;
    }
    
    if ((f_in = fopen(argv[1], "rb")) == NULL)
    {
        perror("Error opening infile");
        return 1;
    }
    
    if ((f_out = fopen(argv[2], "wb")) == NULL)
    {
        perror("Error opening outfile");
        return 1;
    }
    
    s_in = fsize(f_in) - sizeof(zpr_header_t);
    
    fread(&hdr, 1, sizeof(zpr_header_t), f_in);
    
    if (strcmp(hdr.magic, "ZPR") != 0)
    {
        fprintf(stderr, "Error: Invalid magic 0x%02x%02x%02x%02x.\n",
            hdr.magic[0], hdr.magic[1], hdr.magic[2], hdr.magic[3]);
        return 1;
    }
    
    b_in  = malloc(s_in);
    b_out = malloc(hdr.size1);
    
    fread(b_in, 1, s_in, f_in);
    fclose(f_in);
    
    s_out = uncompress(b_in, b_out);

    if ((crc = crc_calculate(b_out, s_out)) != hdr.crc)
    {
        fprintf(stderr, "Error: Invalid CRC (calculated 0x%08x, stored 0x%08x)\n", crc, hdr.crc);
        return 1;
    }

    fwrite(b_out, 1, s_out, f_out);
    fclose(f_out);
    
    free(b_in);
    free(b_out);
    
    printf("Successfully decompressed %s (0x%x bytes) to %s (0x%x bytes)\n",
        argv[1], s_in, argv[2], s_out);
    
    return 0;
}
