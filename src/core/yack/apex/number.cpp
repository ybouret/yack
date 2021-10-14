
#include "yack/apex/number.hpp"
#include "yack/system/wtime.hpp"

namespace yack
{

    namespace apex
    {

        uint64_t number:: add_ticks = 0;
        uint64_t number:: sub_ticks = 0;
        uint64_t number:: lmul_ticks = 0;


        size_t number:: add_count  = 0;
        size_t number:: sub_count  = 0;
        size_t number:: lmul_count = 0;



        void number::reset_tracking() throw()
        {
            add_ticks = 0; add_count = 0;
            sub_ticks = 0; sub_count = 0;
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


        number:: number() throw()
        {
        }

        number:: ~number() throw()
        {
        }
        

    }

}

