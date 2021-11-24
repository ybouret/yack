
//! \file

#ifndef YACK_ROOT_ZBIS_INCLUDED
#define YACK_ROOT_ZBIS_INCLUDED 1

#include "yack/math/root/zroot.hpp"
#include <cmath>

namespace yack
{
    
    namespace math
    {
        
        template <typename T>
        class zbis : public zalgo<T>
        {
        public:
            inline virtual ~zbis() throw() {}
            inline explicit zbis() throw() : zalgo<T>() {}
            using zroot::s;
            
            template <typename FUNCTION> inline
            bool operator()(FUNCTION &F, triplet<T> &x, triplet<T> &f)
            {
                static const T half(0.5);
                switch( this->initialize(x,f) )
                {
                    case zroot::failure: return false;
                    case zroot::success: return true;
                    case zroot::compute:
                        break;
                }
                
                assert(x.a<=x.c);
                assert(s.a!=__zero__);
                assert(s.c!=__zero__);
                T  width = fabs(x.c-x.a);
                
                switch(s.b = __sign::of(f.b = F(x.b = half*(x.a+x.c))))
                {
                    case __zero__: return true; //!< early return
                }
                
                return false;
            }
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zbis);
        };
        
    }
    
}

#endif

