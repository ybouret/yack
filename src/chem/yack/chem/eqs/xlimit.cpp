#include "yack/chem/eqs/xlimit.hpp"

namespace yack
{
    namespace chemical
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


        bool xlimit:: search(const index_level       I,
                             const actors           &A,
                             const readable<double> &C)
        {

            reset();

            for(const actor *a=A.head;a;a=a->next)
            {
                const species &s = **a;
                const double   c = C[s.indx[I]];
                if(c>=0)
                    insert(c/a->nu,s);
            }

            return size>0;
        }

        bool xlimit:: search(const actors           &A,
                             const readable<double> &C,
                             const readable<bool>   &R)
        {
            reset();
            for(const actor *a=A.head;a;a=a->next)
            {
                const species &s = **a; if(!R[s.indx[top_level]]) continue;
                const double   c = C[s.indx[sub_level]];
                if(c>=0)
                    insert(c/a->nu,s);
            }
            return size>0;

        }

        
    }

}
