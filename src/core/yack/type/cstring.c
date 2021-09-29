
#include "yack/type/cstring.h"
#include <string.h>

size_t yack_cstring_size(const char *buffer)
{
    return buffer ? strlen(buffer) : 0;
}

void yack_cstring_copy(char        *buffer,
                       const size_t buflen,
                       const char  *msg,
                       const size_t len)
{
    assert(!(NULL==buffer&&buflen>0));
    assert(!(NULL==msg&&len>0));

    if(buflen>0)
    {
        const size_t maxlen = buflen-1;
        const size_t length = (len<=maxlen) ? len : maxlen;
        memcpy(buffer,msg,length);
        buffer[length] = '\0';
    }


}

void yack_cstring_trim(char *buffer, const char *bad, const size_t num)
{
    size_t buflen = yack_cstring_size(buffer);
    assert(!(NULL==bad&&num>0));
    if(num>0 && buflen>0)
    {
        while(buflen-- > 0)
        {
            char      *p     = &buffer[buflen];
            const char c     = *p;
            int        found = 0;
            for(size_t i=0;i<num;++i)
            {
                if(c==bad[i]) { found=1; break; }
            }
            if(found)
            {
                *p='\0';
                continue;
            }
            else
            {
                break;
            }
        }
    }

}
