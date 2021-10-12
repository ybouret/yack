#include <cstdio>
#include <cstring>
#include <cstdlib>

int main(int argc, char *argv[])
{
    int  count = 0;
    if(argc>1)
    {
        count = atoi(argv[1]);
    }

    char buffer[512];
    while( memset(buffer,0,sizeof(buffer)), fgets(buffer,sizeof(buffer)-1, stdin) )
    {
        int buflen = int(strlen(buffer));
        while(buflen>0)
        {
            char &c = buffer[buflen-1];
            if(c=='\r'||c=='\n')
            {
                c=0;
                --buflen;
                continue;
            }
            break;
        }
        if(buflen<=count)
        {
            fputs(buffer,stdout);
            fputc('\n', stdout);
            fflush(stdout);
        }
    }

    return 0;
}

