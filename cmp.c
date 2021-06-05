#include "cmp.h"

static int get_next_byte(compression_ctx_t *ctx)
{
    return *ctx->data++ ^ XOR_KEY;
}

static int get_next_ctrl(compression_ctx_t *ctx)
{
    int ctrl;
    
    if (ctx->bits_left-- == 1)
    {
        ctx->ctrl_byte = get_next_byte(ctx);
        ctx->bits_left = 8;
    }
    
    ctrl = ctx->ctrl_byte & 1;
    
    ctx->ctrl_byte >>= 1;
    
    return ctrl;
}

int uncompress(uint8_t *in, uint8_t *out)
{
    uint8_t          *start = out;
    compression_ctx_t ctx   = { 1, 0, in };
    
    for (;;)
    {
        int b1, b2, length, distance, next;        
        
        while (get_next_ctrl(&ctx))
            *out++ = get_next_byte(&ctx);
        
        if (get_next_ctrl(&ctx) == 0)
        {
            next     = get_next_ctrl(&ctx);
            length   = (next * 2) + 2 + get_next_ctrl(&ctx);
            distance = get_next_byte(&ctx) - 0x100;
            goto lookback_copy;
        }
        
        b1 = get_next_byte(&ctx);
        b2 = get_next_byte(&ctx);
        
        if (b1 == 0 && b2 == 0)
            break;
        
        distance = (b2 << 5) + (b1 >> 3) - 0x2000;
        
        if (b1 & 7)
            length = (b1 & 7) + 2;
        else
            length = get_next_byte(&ctx) + 10;
        
lookback_copy:
        while (length--)
            *out++ = out[distance];
    }
    
    return out - start;
}
