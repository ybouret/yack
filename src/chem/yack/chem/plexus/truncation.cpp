#include "yack/chem/plexus.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include <iomanip>

namespace yack
{
    
    namespace chemical
    {
        size_t plexus:: truncation(double &scale, const anode *node)
        {
            YACK_CHEM_PRINTLN("//   <truncation>");
            assert( absolute(1.0-scale) <= 0);

            rstack.free();
            ustack.free();
            for( ;node;node=node->next)
            {
                const species &s  = **node;
                const size_t   j  = *s;
                const double   d  = dC[j];
                if(d<0)
                {
                    const double b = -d;      assert(b>0);
                    const double c = Corg[j]; assert(c>=0);
                    rstack << c/b;
                    ustack << j;
                    if(verbose)
                    {
                        lib.pad(std::cerr <<  "//     checking [" << s << "]",s.name);;
                        std::cerr << " = " << std::setw(14) << c;
                        std::cerr << " / " << std::setw(14) << d;
                        std::cerr << " -> " << rstack.back();
                        std::cerr << std::endl;
                    }

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
                YACK_CHEM_PRINTLN("//     rstack="<<rstack);
                YACK_CHEM_PRINTLN("//     ustack="<<ustack);
            }
            else
            {
                YACK_CHEM_PRINTLN("//     none detected!");
            }
            YACK_CHEM_PRINTLN("//   <truncation/>");
            return count;
        }

    }

}


