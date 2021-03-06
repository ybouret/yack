
#include "yack/concurrent/loop.hpp"
#include "yack/type/utils.hpp"
#include <cmath>

namespace yack
{
    namespace concurrent
    {
        loop:: ~loop() throw()
        {
        }

        loop::loop() throw() :
        object(),
        counted(),
        assembly()
        {
        }


        
        void loop:: run(runnable &obj) throw()
        {
            (*this)(runnable::call,&obj);
        }

        double loop:: efficiency(const double seq, const double par) const throw()
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
