#include "Durex.h"

uint32_t FNV32(const char *s)
{
    uint32_t hash = FNV_OFFSET_32, i;
    for (i = 0; i < strlen(s); i++)
    {
        hash = hash ^ (s[i]);       // xor next byte into the bottom of the hash
        hash = hash * FNV_PRIME_32; // Multiply by prime number found to work well
    }
    return hash;
}
