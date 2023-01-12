#include <cstdio>
#include <cstring>
#include <cstdlib>

int main(int argc, char *argv[])
{
    int  min_len = 1;
    int  max_len = 0;
    if(argc>1)
    {
        max_len = atoi(argv[1]);
    }

    if(argc>2)
    {
        min_len = atoi(argv[2]);
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
        if(min_len<=buflen && buflen<=max_len)
        {
            fputs(buffer,stdout);
            fputc('\n', stdout);
            fflush(stdout);
        }
    }

    return 0;
}

