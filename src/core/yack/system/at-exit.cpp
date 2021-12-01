
#include "yack/system/at-exit.hpp"
#include "yack/system/error.hpp"
#include "yack/type/out-of-reach.hpp"
#include <cstring>
#include <cerrno>

namespace yack
{
    static at_exit at_exit_data[at_exit::capacity] ;
    static bool    at_exit_init = true;
    static size_t  at_exit_size = 0;
    
    static inline void at_exit_call() throw()
    {
        for(size_t i=0;i<at_exit_size;++i)
        {
            at_exit &self = at_exit_data[i];
            assert(self.proc);
            self.proc(self.args);
            out_of_reach::zset(&self,sizeof(at_exit));
        }
    }
    
    static inline
    int at_exit_compare(const void *lhs, const void *rhs) throw()
    {
        const at_exit &L = *static_cast<const at_exit *>(lhs);
        const at_exit &R = *static_cast<const at_exit *>(rhs);
        return L.rank - R.rank;
    }
    
    void  at_exit:: perform(callback proc_,void *args_, const longevity rank_)
    {
        assert(proc_);
        if(at_exit_init)
        {
            memset(at_exit_data,0,sizeof(at_exit_data));
            const int res = atexit(at_exit_call);
            if(res!=0)
            {
                system_error::critical_bsd(ENOMEM,"cannot initialize atexit");
            }
            at_exit_init = false;
        }
        if(at_exit_size>=capacity) system_error::critical_bsd(ENOMEM,"at_exit::capacity exceeded");
        
        for(size_t i=0;i<at_exit_size;++i)
        {
            if(at_exit_data[i].rank == rank_ )
            {
                system_error::critical_bsd(EACCES,"at_exit multiple longevities");
            }
        }
        
        {
            at_exit &self = at_exit_data[at_exit_size++];
            self.proc = proc_;
            self.args = args_;
            self.rank = rank_;
        }
        
        qsort(at_exit_data,at_exit_size,sizeof(at_exit),at_exit_compare);
        
    }
    
}
