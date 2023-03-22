
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

            //------------------------------------------------------------------
            //
            // check upper value
            //
            //------------------------------------------------------------------
            const double          xi    = limitation.extent;   // available
            const zl_node * const upper = tail; assert(tail);  // upper value
            {
                const zlimit &up = **upper;
                switch( __sign::of(xi,up.extent) )
                {
                    case positive:
                        // enough extent for all, stop at upper value
                        correction = up;
                        YACK_XMLOG(xml, yack_success << " complete [gt] solving : " << correction);
                        return true;

                    case __zero__:
                        // enough extent for all, stop at same upper value/limitation
                        correction = limitation;
                        correction.merge_back_copy(up);
                        YACK_XMLOG(xml, yack_success << " complete [eq] solving : " << correction);
                        return true;

                    case negative:
                        if(1==size)
                        {
                            // not enough for the single boundary
                            correction = limitation;
                            YACK_XMLOG(xml, yack_failure << " [lt] single boundary, use : " << correction);
                            return false;
                        }
                        else
                        {
                            // go on with algo
                            break;
                        }

                }
            }

            assert(size>=2);

            //------------------------------------------------------------------
            //
            // check lower value
            //
            //------------------------------------------------------------------
            const zl_node *lower = head; assert(lower!=upper);
            {
                const zlimit &lo = **lower;
                switch( __sign::of(xi,lo.extent) )
                {
                    case negative:
                        // smallest than the lowest boundary => best effort
                        correction = limitation;
                        YACK_XMLOG(xml, yack_failure << " [lt] multiple boundaries, use: " << correction);
                        return false;

                    case __zero__:
                        // equal to the lowest boundary => best effort
                        correction = limitation;
                        correction.merge_back_copy(lo);
                        YACK_XMLOG(xml, yack_failure << " too small [eq] : " << correction);
                        return false;

                    case positive:
                        // generic case
                        break;

                }
            }

            //------------------------------------------------------------------
            //
            // bracket xi
            //
            //------------------------------------------------------------------

        PROBE:
            const zl_node * const probe = lower->next;
            while(probe!=upper)
            {
                const zlimit &next = **probe;
                switch( __sign::of(xi,next.extent) )
                {
                    case negative:
                        // ok, bracketed, will use lower
                        goto DONE;

                    case __zero__:
                        // special numeric case
                        correction = limitation;
                        correction.merge_back_copy(next);
                        YACK_XMLOG(xml, yack_failure << " generic :" << **lower << " < " << correction);
                        return false;

                    case positive:
                        // need to go on
                        break;
                }

                lower = probe;
                goto PROBE;
            }
        DONE:
            assert(lower);
            assert(upper!=lower);
            assert( (**lower).extent < xi);
            assert( lower->next );
            assert( xi < (**(lower->next)).extent );

            correction = **lower;
            YACK_XMLOG(xml, yack_failure << " generic : " << correction);

            return false;
            
        }



    }
}

