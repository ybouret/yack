//! \file

#ifndef YACK_ROOT_ZBIS_INCLUDED
#define YACK_ROOT_ZBIS_INCLUDED 1

#include "yack/math/root/zroot.hpp"
#include <cmath>

namespace yack
{
    
    namespace math
    {

        namespace core
        {
            extern const char zbis_name[]; //!< "zbis"
        }

        //______________________________________________________________________
        //
        //
        //! bisection method
        //
        //______________________________________________________________________
        template <typename T>
        class zbis : public zroot<T>
        {
        public:
            inline virtual            ~zbis() throw() {}                //!< setup
            inline explicit            zbis() throw() : zroot<T>() {}   //!< cleanup
            inline virtual const char *name() const throw() { return core::zbis_name; }

            //! bisection operator, reentrant
            /**
             upon success, f.b = F(x.b) was the last call
             \param F  function to zero
             \param x x.a and x.c must be set
             \param f f.a and f.c must be set
            */
            template <typename FUNCTION> inline
            bool operator()(FUNCTION &F, triplet<T> &x, triplet<T> &f) const
            {
                static const T     half(0.5);
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                triplet<sign_type> s = { __zero__, __zero__, __zero__ };
                switch( this->initialize(F,x,f,s) )
                {
                    case zroot<T>::failure: return false;
                    case zroot<T>::success: return true;
                    case zroot<T>::compute: break;
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
                T  width = std::abs(x.c-x.a);
                T *x_neg = &x.a; T *f_neg = &f.a;
                T *x_pos = &x.c; T *f_pos = &f.c;
                if(positive==s.a)
                {
                    assert(negative==s.c);
                    cswap(x_neg,x_pos);
                    cswap(f_neg,f_pos);
                }
                assert( negative == __sign::of(*f_neg) );
                assert( positive == __sign::of(*f_pos) );

                //______________________________________________________________
                //
                // cycles
                //______________________________________________________________
            CYCLE:
                switch(s.b = __sign::of(f.b = F(x.b=half*(x.a+x.c))))
                {
                    case __zero__: return true; // early return
                    case negative: *x_neg = x.b; *f_neg=f.b; break;
                    case positive: *x_pos = x.b; *f_pos=f.b; break;
                }
                assert(x.is_increasing());
                if(std::abs(f.a-f.c)<=0) return true;
                const T new_width = std::abs(x.c-x.a); if(new_width>=width) return true;
                width = new_width;
                goto CYCLE;
            }
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zbis);
        };
        
    }
    
}

#endif

