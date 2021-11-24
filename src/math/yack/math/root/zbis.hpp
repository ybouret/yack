
//! \file

#ifndef YACK_ROOT_ZBIS_INCLUDED
#define YACK_ROOT_ZBIS_INCLUDED 1

#include "yack/math/root/zroot.hpp"
#include <cmath>

namespace yack
{
    
    namespace math
    {
        
        //______________________________________________________________________
        //
        //
        //! bisection method
        //
        //______________________________________________________________________
        template <typename T>
        class zbis : public zalgo<T>
        {
        public:
            inline virtual ~zbis() throw() {}                //!< setup
            inline explicit zbis() throw() : zalgo<T>() {}   //!< cleanup
            
            //! bisection operator, reentrant
            /**
             upon success, f.b = F(x.b) was the last call
             \param F a function to zero
             \param x x.a and x.c must be set
             \param f f.a and f.c must be set
            */
            template <typename FUNCTION> inline
            bool operator()(FUNCTION &F, triplet<T> &x, triplet<T> &f) const
            {
                static const T     h(0.5);
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                triplet<sign_type> s = { __zero__, __zero__, __zero__ };
                switch( this->initialize(F,x,f,s) )
                {
                    case zroot::failure: return false;
                    case zroot::success: return true;
                    case zroot::compute: break;
                }
                
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(x.a<=x.c);
                assert(s.a!=__zero__);
                assert(s.c!=__zero__);
                assert(s.a!=s.c);
                
                //______________________________________________________________
                //
                // initialize search
                //______________________________________________________________
                T  width = fabs(x.c-x.a);
                T *x_neg = &x.a, *x_pos = &x.c;
                T *f_neg = &f.a, *f_pos = &f.c;
                if(positive==s.a)
                {
                    assert(negative==s.c);
                    cswap(x_neg,x_pos);
                    cswap(f_neg,f_pos);
                }
                
                //______________________________________________________________
                //
                // cycles
                //______________________________________________________________
            CYCLE:
                switch(s.b = __sign::of(f.b = F(x.b = h*(x.a+x.c))))
                {
                    case __zero__: return true; // early return
                    case negative: *x_neg = x.b; *f_neg=f.b; break;
                    case positive: *x_pos = x.b; *f_pos=f.b; break;
                }
                assert(x.a<=x.c);
                const T new_width = fabs(x.c-x.a);
                if(new_width>=width)
                    return true;
                width = new_width;
                goto CYCLE;
            }
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zbis);
        };
        
    }
    
}

#endif

