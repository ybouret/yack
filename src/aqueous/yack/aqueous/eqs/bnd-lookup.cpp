
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

            //------------------------------------------------------------------
            //
            // compare with lowest boundary
            //
            //------------------------------------------------------------------
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
                            YACK_XMLOG(xml, "success (by matching unique): " << correction);
                            return true;
                        }
                        else
                        {
                            // failure if remaining boudnaries
                            YACK_XMLOG(xml, "failure (but equal to lower): " << correction);
                            return false;
                        }

                    case positive:
                        if(1==size)
                        {
                            correction = lo;
                            YACK_XMLOG(xml, "success (greater than unique): " << correction);
                            return true;
                        }
                        break;
                }
            }

            //------------------------------------------------------------------
            //
            // compare with highest boundary
            //
            //------------------------------------------------------------------
            assert(size>=2);
            const zl_node * const upper = tail; assert(upper!=lower);
            {
                const zlimit &up = **upper;
                switch( __sign::of(xi,up.extent) )
                {
                    case negative:
                        // need to look up
                        break;

                    case __zero__:
                        // special win case
                        correction = limitation;
                        correction.merge_back_copy(up);
                        YACK_XMLOG(xml, "success (by matching upper): " << correction);
                        return true;

                    case positive:
                        // best case scenario
                        correction = up;
                        YACK_XMLOG(xml, "success (greater than upper): " << correction);
                        return true;
                }
            }

            assert(lower);
            assert(upper);
            assert(lower!=upper);

        PROBE:
            const zl_node * const probe = lower->next;
            if(probe!=upper)
            {

                lower = probe;
                goto PROBE;
            }
            std::cerr << "not implemented" << std::endl;



            
            return false;
        }


    }
}

