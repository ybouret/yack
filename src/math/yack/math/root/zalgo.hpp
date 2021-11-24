
//! \file

#ifndef YACK_ROOT_ZALGO_INCLUDED
#define YACK_ROOT_ZALGO_INCLUDED 1

#include "yack/math/triplet.hpp"
#include "yack/signs.hpp"

namespace yack
{
    
    namespace math
    {

        template <typename T>
        class zalgo
        {
        public:
            
            bool init( triplet<T> &x, triplet<T> &f)
            {
                if(x.a>x.c)
                {
                    cswap(x.a,x.c);
                    cswap(f.a,f.c);
                }
                switch( __sign::pair( __sign::of(f.a), __sign::of(f.b) ) )
                {
                        
                }
                
            }
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zalgo);
        };
        
    }
    
}

#endif

