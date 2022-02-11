
#include "yack/chem/plexus.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"

namespace yack
{
    
    namespace chemical
    {
        size_t plexus:: findTruncation(double &scale)
        {
            YACK_CHEM_PRINTLN("// [truncation]");
            assert( absolute(1.0-scale) <= 0);

            rstack.free();
            ustack.free();
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &sp = ***node;
                const size_t   j  = sp.indx;
                const double   d  = dC[j];
                if(d<0)
                {
                    YACK_CHEM_PRINTLN("//    checking " << sp);
                    const double b = -d;      assert(b>0);
                    const double c = Corg[j]; assert(c>=0);
                    rstack << c/b;
                    ustack << j;
                }
            }


            size_t count = 0;
            if(rstack.size())
            {
                hsort(rstack,ustack,comparison::increasing<double>);
                scale = rstack.front();
                count = 1;
                while(count<rstack.size() && rstack[count+1] <= scale) ++count;
                while(rstack.size()>count)
                {
                    rstack.pop_back();
                    ustack.pop_back();
                }
                YACK_CHEM_PRINTLN("// rstack="<<rstack);
                YACK_CHEM_PRINTLN("// ustack="<<ustack);
            }
            return count;
        }

    }

}


