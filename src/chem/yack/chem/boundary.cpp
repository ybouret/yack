#include "yack/chem/boundary.hpp"

namespace yack
{
    namespace chemical
    {
        boundary:: ~boundary() throw()
        {
        }
        
        boundary:: boundary(const sp_fund &io) throw() :
        sp_repo(io),
        xi(0)
        {
        }
        
        void boundary:: free() throw() {
            release();
            xi = 0;
        }
        
        void boundary:: add(const double   x,
                            const species &s)
        {
            assert(x>=0);
            sp_repo &self = *this;
            if(size<=0) {
                xi   =   x;
                self << &s;
                return;
            }
            switch( __sign::of(x,xi) )
            {
                case negative: assert(x<xi);
                    release();
                    xi   =   x;
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
