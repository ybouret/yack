
#include "yack/aqueous/eqs/boundaries.hpp"

namespace yack
{
    namespace aqueous
    {
        bool boundaries:: check_valid() const noexcept
        {
            if(size<=0)  { std::cerr << "empty boudaries" << std::endl; return false; }
            if(!check()) return false;
            for(const zl_node *node=head;node;node=node->next)
            {
                if( (**node).extent <= 0 )
                {
                    std::cerr << "extent<=0 in boundaries" << std::endl;;
                    return false;
                }
            }
            return true;
        }


        bool boundaries:: lookup(zlimit       &correction,
                                 const zlimit &limitation) const
        {
            //------------------------------------------------------------------
            //
            // starting with a non empty list of positive boundaries
            //
            //------------------------------------------------------------------
            assert(check_valid());
            assert(limitation.extent>=0);
            correction.reset();

            std::cerr << "boundaries: " << (*this)    << std::endl;
            std::cerr << "limitation: " << limitation << std::endl;

            const double xi = limitation.extent;
            const zl_node *lower = head; assert(lower);
            {
                const zlimit &lo = **lower;
                switch( __sign::of(xi,lo.extent) )
                {
                    case negative:
                        // xi is too small, best effort
                        correction = limitation;
                        return false;

                    case __zero__:
                        // xi can nullify lower boundary
                        correction = limitation;
                        correction.merge_back_copy(lo);
                        return 1==size; // success if only one boundary

                    case positive:
                        if(1==size)
                        {
                            correction = lo;
                            return true;
                        }
                        break;
                }
            }

            assert(size>=2);


            
            return false;
        }


    }
}

