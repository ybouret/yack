
#include "yack/concurrent/loop.hpp"
#include "yack/type/utils.hpp"
#include <cmath>

namespace yack
{
    namespace concurrent
    {
        loop:: ~loop() noexcept
        {
        }

        loop::loop() noexcept :
        object(),
        counted(),
        assembly()
        {
        }


        
        void loop:: run(runnable &obj) noexcept
        {
            (*this)(runnable::call,&obj);
        }

        double loop:: efficiency(const double seq, const double par) const noexcept
        {
            assert(seq>0);
            assert(par>0);
            const double the_speed_up = par/seq;
            const double max_speed_up = size();
            const double percent = min_of<double>(floor(10000.0*the_speed_up/max_speed_up+0.5)/100.0,100);
            return percent;
        }


      

    }

}
