#include "yack/mpi++/mpi.hpp"
#include "yack/string.hpp"
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdio>

#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"


namespace yack
{

    void mpi:: Printf(FILE *fp, const char *fmt, ...) const
    {
        static const char fn[] = "mpi::Printf";
        assert(fmt!=NULL);
        if(primary)
        {
            assert(fp!=NULL);
            // direct printf
            va_list ap;
            va_start(ap,fmt);
            const int res = vfprintf(fp,fmt,ap);
            va_end(ap);
            if(res<0) throw libc::exception(errno,"%s(...)",fn);

            for(int r=1;r<size;++r)
            {
                const int len = Recv<int>(r,io_tag);
                if(len>0)
                {
                    string  ans(len,as_capacity,true);
                    void   *ptr = &ans[1];
                    Recv(ptr,len, MPI_BYTE,1,r,io_tag);
                    fwrite(ptr,len,1,fp);
                }
            }
            fflush(fp);
            
        }
        else
        {
            int res = 0;
            {
                va_list ap;
                va_start(ap,fmt);
                res = vsnprintf(NULL,0,fmt,ap);
                va_end(ap);
                if(res<0) throw libc::exception(errno,"%s(...)",fn);
            }

            Send(res,0,io_tag);

            if(res>0)
            {
                string  ans(res,as_capacity,true); assert( ans.size() == size_t(res) );
                va_list ap;
                va_start(ap,fmt);
                const int chk = vsnprintf(&ans[1],ans.size()+1,fmt,ap);
                va_end(ap);
                if(res!=chk) throw  yack::exception("%s length mismatch!",fn);
                Send(ans(),res,MPI_BYTE,1,0,io_tag);
            }
        }
    }

}
