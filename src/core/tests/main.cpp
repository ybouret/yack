#include "yack/setup.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    if(true)
    {
        /* output CRC table. */
        for(uint32_t i   = 0; i < 256; i++)
        {
            uint32_t rem = i;  /* remainder from polynomial division */
            for(uint32_t j = 0; j < 8; j++)
            {
                if (rem & 1)
                {
                    rem >>= 1;
                    rem ^= 0xedb88320;
                } else
                    rem >>= 1;
            }
            fprintf(stdout," 0x%08x", unsigned(rem) );
            if(i<255)            fprintf(stdout,",");
            if(0==( (i+1) % 16)) fprintf(stdout,"\n");
        }
    }

    return 0;
}
