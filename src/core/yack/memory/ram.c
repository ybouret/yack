
#include "yack/memory/ram.h"
#include "yack/system/error.h"
#include <errno.h>

static uint64_t yack_ram = 0;

uint64_t yack_ram_get()
{
    return yack_ram;
}

static
void yack_ram_check_more_than(const size_t count)
{
    if(count>yack_ram) yack_bsd_critical_error(EACCES,"yack_ram_release(more than allocated)");
}

#include <stdio.h>

void * yack_ram_acquire(size_t *count,  const size_t block_size)
{
    assert(NULL!=count);
    assert(0<block_size);
    if(*count<=0)
    {
        return NULL;
    }
    else
    {
        void *addr = calloc(*count,block_size);

        if(addr)
        {
            yack_ram  += ( *count *= block_size );
            return addr;
        }
        else
        {
            *count = 0;
            return NULL;
        }
    }

}


void yack_ram_release(void **handle, size_t *count)
{
    assert(NULL!=handle);
    assert(NULL!=count);

    if(*handle)
    {
        assert(0<*count);
        yack_ram_check_more_than(*count);
        free(*handle);
        yack_ram  -= *count;
        *handle = NULL;
        *count  = 0;
    }
    else
    {
        assert(0==*count);
    }
}
