
#include "yack/apex/number.hpp"
#include "yack/system/wtime.hpp"

namespace yack
{

    namespace apex
    {

        uint64_t number:: add_ticks  = 0;
        uint64_t number:: sub_ticks  = 0;
        uint64_t number:: lmul_ticks = 0;
        uint64_t number:: fmul_ticks = 0;
        uint64_t number:: fsqr_ticks = 0;
        uint64_t number:: div_ticks  = 0;
        uint64_t number:: mod_ticks  = 0;

        size_t number:: add_count  = 0;
        size_t number:: sub_count  = 0;
        size_t number:: lmul_count = 0;
        size_t number:: fmul_count = 0;
        size_t number:: fsqr_count = 0;
        size_t number:: div_count  = 0;
        size_t number:: mod_count  = 0;



        void number::reset_tracking() throw()
        {
            add_ticks  = 0; add_count = 0;
            sub_ticks  = 0; sub_count = 0;
            div_ticks  = 0; div_count = 0;
            mod_ticks  = 0; mod_count = 0;
            lmul_ticks = 0; lmul_count = 0;
            fmul_ticks = 0; fmul_count = 0;
            fsqr_ticks = 0; fsqr_count = 0;

        }


        double number:: rate(const uint64_t ell,
                             const size_t   num)
        {
            if(ell>0)
            {
                wtime st;
                const double dt = st(ell);
                return 1.0e-6*(num/dt);
            }
            else
            {
                return -1;
            }
        }

        double number:: add_rate()
        {
            return rate(add_ticks,add_count);
        }

        double number:: sub_rate()
        {
            return rate(sub_ticks,sub_count);
        }

        double number:: lmul_rate()
        {
            return rate(lmul_ticks,lmul_count);
        }

        double number:: fmul_rate()
        {
            return rate(fmul_ticks,fmul_count);
        }

        double number:: fsqr_rate()
        {
            return rate(fsqr_ticks,fsqr_count);
        }

        double number:: div_rate()
        {
            return rate(div_ticks,div_count);
        }

        double number:: mod_rate()
        {
            return rate(mod_ticks,mod_count);
        }


        number:: number() throw()
        {
        }

        number:: ~number() throw()
        {
        }
        

    }

}


#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{

    namespace apex
    {
        void number:: cast_overflow(const char *who) const
        {
            throw libc::exception(ERANGE,"%s::cast_to('%s')", class_uid(), (who?who:yack_unknown) );
        }
    }

}

