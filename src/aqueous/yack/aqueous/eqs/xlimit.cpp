
#include "yack/aqueous/eqs/xlimit.hpp"

namespace yack
{
    namespace aqueous
    {

        xlimit:: ~xlimit() noexcept
        {
        }

        xlimit:: xlimit(const sp_proxy &_) noexcept :
        zlimit(_)
        {
        }





        void xlimit:: insert(const double   xi,
                             const species &sp)
        {
            assert(xi>=0);
            if(size<=0)
            {
                extent   = xi;
                (*this) << sp;
                return;
            }
            else
            {
                switch( __sign::of(xi,extent) )
                {
                        //------------------------------------------------------
                        // new winner
                        //------------------------------------------------------
                    case negative: assert(xi<extent);
                        extent = xi;
                        clear();
                        (*this) << sp;
                        break;

                        //------------------------------------------------------
                        // ex-aequo
                        //------------------------------------------------------
                    case __zero__:
                        (*this) << sp;
                        break;

                        //------------------------------------------------------
                        // looser
                        //------------------------------------------------------
                    case positive:
                        break;
                }
            }

        }


        bool xlimit::search(const actors           &A,
                            const readable<double> &C)
        {

            reset();

            for(const actor *a=A.head;a;a=a->next)
            {
                const species &s = **a;
                const double   c = C[s.indx[0]];
                if(c>=0)
                    insert(c/a->nu,s);
            }

            return size>0;
        }


#if 0
        bool xlimit:: get_extent(const actors           &A,
                                 const readable<double> &C)
        {
            // initialize
            reset();

            // find first positive extent
            const actor *a = A.head;
            for(;a;a=a->next)
            {
                const species &s = **a;
                const double   c = C[s.indx[0]];
                if(c>=0)
                {
                    extent = c/a->nu;
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
                    switch( __sign::of(x,extent) )
                    {

                        case negative: assert(x<extent); // new winner
                            extent = x; clear(); (*this) << s;
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
#endif
        
    }

}

