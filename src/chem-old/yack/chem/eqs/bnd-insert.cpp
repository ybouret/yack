#include "yack/chem/eqs/boundaries.hpp"

namespace yack
{
    namespace chemical
    {
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
            assert(xi>0);
            
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
                    case __zero__: zcurr << sp; assert(check()); return; // equal to probe
                    case positive: break;                                // check next interal
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

            (void) insert_after(lower,cache->create(spore,xi,sp));
            assert(check());
        }

    }

}


