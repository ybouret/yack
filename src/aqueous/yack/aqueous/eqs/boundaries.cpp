
#include "yack/aqueous/eqs/boundaries.hpp"

namespace yack
{
    namespace aqueous
    {
        
        boundaries:: ~boundaries() noexcept
        {

        }

        boundaries:: boundaries(const zl_proxy &zlp,
                                const sp_proxy &spp) noexcept :
        zlimits(zlp),
        spore(spp)
        {
        }


        bool boundaries:: check() const noexcept
        {
            if(size>=2)
            {
                for(const zl_node *node=head, *next=head->next;next;node=next,next=next->next)
                {
                    if( (**node).extent >= (**next).extent ) return false;
                }
            }
            return true;
        }

        void boundaries:: insert1(const double xi, const species &sp)
        {
            assert(1==size);
            zlimit &zl = **head;
            switch( __sign::of(xi,zl.extent) )
            {
                case negative: (void) shove(spore,xi,sp); assert(check()); return; // lt
                case positive: (void) annex(spore,xi,sp); assert(check()); return; // gt
                case __zero__: zl << sp;                  assert(check()); return; // eq
            }
        }

        void  boundaries:: insert(const double   xi,
                                  const species &sp)
        {
            assert(xi<0);

            //------------------------------------------------------------------
            //
            // preparing insertion
            //
            //------------------------------------------------------------------
            switch(size)
            {
                case 0: (void) annex(spore,xi,sp); assert(check()); return; //!< initialize with new node
                case 1: insert1(xi,sp);            assert(check()); return; //!< special case
                default:
                    break;
            }

            //------------------------------------------------------------------
            //
            // default alogrithm
            //
            //------------------------------------------------------------------
            assert(size>=2);

            //------------------------------------------------------------------
            // check agains head
            //------------------------------------------------------------------
            zl_node *lower = head;
            {
                zlimit &zhead = **lower;
                switch( __sign::of(xi,zhead.extent) )
                {
                    case negative: (void) shove(spore,xi,sp); return; // lt lower
                    case __zero__: zhead << sp;               return; // eq lower
                    case positive: break;                             // gt lower => break
                }
            }

            //------------------------------------------------------------------
            // check agains tail
            //------------------------------------------------------------------
            const zl_node *upper = tail; assert(upper!=lower);
            {
                zlimit &ztail = coerce(**upper);
                switch( __sign::of(xi,ztail.extent) )
                {
                    case positive: (void) annex(spore,xi,sp); assert(check()); return; // gt upper
                    case __zero__: ztail << sp;               assert(check()); return; // eq upper
                    case negative: break;                                              // lt upper => break
                }
            }

            //------------------------------------------------------------------
            // initialize probe and loop until located
            //------------------------------------------------------------------
            zl_node *probe = lower->next; assert(probe!=NULL);
            while(probe!=upper)
            {
                zlimit &zcurr = **probe;

                switch( __sign::of(xi,zcurr.extent) )
                {
                    case negative: goto LOCATED;                         // between lower and probe
                    case __zero__: zcurr << sp; assert(check()); return; // eq probe
                    case positive: break;
                }

                lower=probe;
                probe=probe->next;
            }

            //------------------------------------------------------------------
            // located
            //------------------------------------------------------------------
        LOCATED:
            assert( (**lower).extent < xi );
            assert( xi < (**probe).extent );
            //std::cerr << "located " << xi << " between " << (**lower).extent << " and " << (**probe).extent << std::endl;

            (void) insert_after(lower,cache->create(spore,xi,sp));
            assert(check());
        }

        bool boundaries:: check_valid() const noexcept
        {
            if(size<=0)  { std::cerr << "empty boudaries" << std::endl; return false; }
            if(!check()) return false;
            for(const zl_node *node=head;node;node=node->next)
            {
                if( (**node).extent >=0 )
                {
                    std::cerr << "extent>=0 in boundaries" << std::endl;;
                    return false;
                }
            }
            return true;
        }


        bool boundaries:: find(zlimit       &correction,
                               const zlimit &limitation) const
        {
            // starting with a non empty list of negative boundaries
            assert(check_valid());
            assert(limitation.extent>=0);
            correction.reset();

            std::cerr << "boundaries: " << (*this)    << std::endl;
            std::cerr << "limitation: " << limitation << std::endl;

            // locating xi
            const double xi = -limitation.extent;

            // testing upper bound
            const zl_node *upper = tail;
            {
                const zlimit &up = **upper;
                switch( __sign::of(up.extent,xi) )
                {

                    case positive: assert(xi>up.extent);
                        // couldn't correct any, best effort is limitation
                        correction = limitation;
                        return false;

                    case __zero__:
                        // matching upper bound
                        correction = limitation;
                        correction.merge_back_copy(up);
                        return (1 == size); // done if only one!

                    case negative:
                        if(1==size)
                        {
                            correction = up; assert(correction.extent<0);
                            opposite(correction.extent);
                            return true;
                        }
                        break;

                }
            }







            return false;
        }


    }
}
