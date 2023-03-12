
#include "yack/aqueous/eqs/xlimit.hpp"

namespace yack
{
    namespace aqueous
    {

        xlimit:: ~xlimit() noexcept
        {
        }

        xlimit:: xlimit(const sp_proxy &_) noexcept :
        sp_repo(_),
        xi(0)
        {
        }

        void xlimit:: initialize() noexcept
        {
            clear();
            xi = 0;
        }

        std::ostream & operator<<(std::ostream &os, const xlimit &self)
        {
            const sp_repo &repo = self;
            os << repo << " @" << self.xi;
            return os;
        }

        bool xlimit:: get_extent(const actors           &A,
                                 const readable<double> &C)
        {
            // initialize
            initialize();

            // find first positive extent
            const actor *a = A.head;
            for(;a;a=a->next)
            {
                const species &s = **a;
                const double   c = C[s.indx[0]];
                if(c>=0)
                {
                    xi = c/a->nu;
                    (*this) << s;
                    a=a->next;
                    break;
                }
            }

            // find LEQ extent
            for(;a;a=a->next)
            {
                const species &s = **a;
                const double   c = C[s.indx[0]];
                if(c>=0)
                {
                    const double x = c/a->nu;
                    switch( __sign::of(x,xi) )
                    {

                        case negative: assert(x<xi); // new winner
                            xi = x; clear(); (*this) << s;
                            break;

                        case __zero__:
                            (*this) << s; // ex-aequp
                            break;

                        case positive:   // do nothing
                            break;
                    }
                }
            }

            if(size>0)
            {
                return true;
            }
            else
            {
                return false;
            }

        }

        
    }

}

