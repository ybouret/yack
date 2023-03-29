#include "yack/chem/realm/collector.hpp"

namespace yack
{

    namespace chemical
    {
        
        collector:: ~collector() noexcept
        {

        }

        collector:: collector(const size_t n, const size_t m) :
        cxx_array<double>(m),
        collecting::caches(),
        charts(n,*this),
        balanced(eqp),
        solvable(eqp),
        weakened(eqp),
        singular(eqp),
        solo(zlp,spp),
        gain(n,0),
        Cbal(n,n>0?m:n),
        xadd()
        {
        }

        void collector:: initialize() noexcept
        {
            balanced.clear();
            solvable.clear();
            weakened.clear();
            singular.clear();
            gain.ld(-1);
        }


       
      

      


    }
    
}


