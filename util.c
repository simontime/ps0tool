#include "util.h"

size_t fsize(FILE *f)
{
    size_t sz;
    
    fseek(f, 0, SEEK_END);
    sz = ftell(f);
    rewind(f);
    
    return sz;
}
