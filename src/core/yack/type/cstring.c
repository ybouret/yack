
#include "yack/type/cstring.h"
#include <string.h>

size_t yack_cstring_size(const char *buffer)
{
    return buffer ? strlen(buffer) : 0;
}

void yack_cstring_strcpy(char        *buffer,
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

static size_t yack_cstring_safelength(char *buffer, const size_t buflen)
{
    const size_t maxlen = buflen-1;
    assert(NULL!=buffer);
    assert(buflen>0);
    for(size_t i=0;i<buflen;++i)
    {
        if(0==buffer[i]) return i;
    }
    buffer[maxlen] = '\0';
    return maxlen;

}


void   yack_cstring_strcat(char *buffer, const size_t buflen, const char *msg, const size_t len)
{
    assert(!(NULL==buffer&&buflen>0));
    assert(!(NULL==msg&&len>0));
    if(buflen>0&&len>0)
    {
        const size_t maxlen = buflen-1;
        const size_t curlen = yack_cstring_safelength(buffer,buflen);
        const size_t space  = maxlen-curlen;
        assert(curlen<=maxlen);
        assert(0==buffer[curlen]);
        if(space>0)
        {
            const size_t bytes  = space<len ? space:len;
            char *       target = buffer+curlen;
            assert(curlen+bytes<buflen);
            memcpy(target,msg,bytes);
            target[bytes] = 0;
        }
    }

}

void   yack_cstring_chrcpy(char *buffer, const size_t buflen, const char c)
{
    yack_cstring_strcpy(buffer,buflen,&c,1);
}

void   yack_cstring_chrcat(char *buffer, const size_t buflen, const char c)
{
    yack_cstring_strcat(buffer,buflen,&c,1);
}

void   yack_cstring_msgcpy(char *buffer, const size_t buflen, const char *msg)
{
    yack_cstring_strcpy(buffer,buflen,msg,yack_cstring_size(msg));
}

void   yack_cstring_msgcat(char *buffer, const size_t buflen, const char *msg)
{
    yack_cstring_strcat(buffer,buflen,msg,yack_cstring_size(msg));
}
