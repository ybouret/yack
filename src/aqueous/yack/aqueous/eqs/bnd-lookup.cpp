
#include "yack/aqueous/eqs/boundaries.hpp"
#include "yack/ios/xmlog.hpp"

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


        bool boundaries:: lookup(const xmlog  &xml,
                                 zlimit       &correction,
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

            YACK_XMLOG(xml, "look up " << limitation << " in " << *this);



            const double xi = limitation.extent;
            const zl_node *lower = head; assert(lower);
            {
                const zlimit &lo = **lower;
                switch( __sign::of(xi,lo.extent) )
                {
                    case negative:
                        // xi is too small, best effort
                        correction = limitation;
                        YACK_XMLOG(xml, "failure (too small) : best effort is " << correction);
                        return false;

                    case __zero__:
                        // xi can nullify lower boundary
                        correction = limitation;
                        correction.merge_back_copy(lo);
                        if(1==size)
                        {
                            // success if only one boundary
                            YACK_XMLOG(xml, "success (matching unique): " << correction);
                            return true;
                        }
                        else
                        {
                            // failure if remaining boudnaries
                            YACK_XMLOG(xml, "failure (matching first): " << correction);
                            return false;
                        }

                    case positive:
                        if(1==size)
                        {
                            YACK_XMLOG(xml, "success (more than first): " << correction);
                            correction = lo;
                            return true;
                        }
                        break;
                }
            }

            assert(size>=2);
            std::cerr << "not implemented" << std::endl;

            
            return false;
        }


    }
}

