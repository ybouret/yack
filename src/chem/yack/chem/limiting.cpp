
#include "yack/chem/limiting.hpp"

namespace yack
{
    namespace chemical
    {
        limiting:: ~limiting() throw()
        {
        }

        limiting:: limiting(const sp_fund &io) throw() :
        frontier(io)
        {
        }



        void limiting:: upgrade(const double   x,
                                const species &s)
        {
            assert(x>=0);
            sp_repo &self = *this;

            // initialize first
            if(size<=0) {
                coerce(xi) = x;
                self << &s;
                return;
            }

            // update with any other value
            assert(size>0);
            switch( __sign::of(x,xi) )
            {
                case negative: assert(x<xi);
                    release();
                    coerce(xi) =  x;
                    self << &s;
                    break;

                case __zero__:
                    self << &s;
                    break;

                case positive: assert(x>xi);
                    break;
            }
        }


    }

}
