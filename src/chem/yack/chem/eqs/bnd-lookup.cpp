
#include "yack/chem/eqs/boundaries.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack
{
    namespace chemical
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


        shift_status boundaries:: try_shift(const xmlog  &xml,
                                            zlimit       &correction,
                                            const zlimit &limitation) const
        {
            //------------------------------------------------------------------
            //
            // starting with a non empty list of positive boundaries
            // and a positive or null limitation
            //
            //------------------------------------------------------------------
            assert(check_valid());
            assert(limitation.extent>=0);
            correction.reset();

            static const char pfx[] = "|_";
            static const char sep[] = " : ";

            YACK_XMLOG(xml, pfx << "look up : " << limitation << " in " << *this);
            if(limitation.size<=0 || fabs(limitation.extent)<=0)
            {
                YACK_XMLOG(xml, pfx << yack_failure << sep << correction << ", no available shift");
                return shift_blocked;
            }
            
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
                        YACK_XMLOG(xml, pfx << yack_success << sep << correction << ", complete solving");
                        return shift_success;

                    case __zero__:
                        // enough extent for all, stop at ex-aequo upper value/limitation
                        correction = limitation;
                        correction.merge_back_copy(up);
                        YACK_XMLOG(xml, pfx << yack_success << sep << correction << ", complete solving (ex-aequo)");
                        return shift_success;

                    case negative:
                        if(1==size)
                        {
                            // not enough for the single boundary
                            correction = limitation;
                            YACK_XMLOG(xml, pfx << yack_failure << sep << correction << ", too small for single boundary");
                            return shift_partial;
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
                        YACK_XMLOG(xml, pfx << yack_failure << sep << correction << ", too small for multiple boundaries");
                        return shift_partial;

                    case __zero__:
                        // equal to the lowest boundary => best effort
                        correction = limitation;
                        correction.merge_back_copy(lo);
                        YACK_XMLOG(xml, pfx << yack_failure << sep << correction << ", too small but matching first of multiple boundaries");
                        return shift_partial;

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
                        YACK_XMLOG(xml, pfx << yack_failure << sep << correction << ", too small but matching one of multiple boundaries");
                        return shift_partial;

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
            YACK_XMLOG(xml, pfx << yack_failure << sep << correction << ", too small (default case)");

            return shift_partial;
            
        }



    }
}

