
//! \file

#ifndef YACK_ROOT_ZRID_INCLUDED
#define YACK_ROOT_ZRID_INCLUDED 1

#include "yack/math/root/zroot.hpp"
#include <cmath>

namespace yack
{

    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! Ridder's method
        //
        //______________________________________________________________________
        template <typename T>
        class zrid : public zroot<T>
        {
        public:
            inline virtual ~zrid() throw() {}                //!< setup
            inline explicit zrid() throw() : zroot<T>() {}   //!< cleanup

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
                static const T     h(0.5);
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
                std::cerr << "df: " << fabs(f.a-f.c) << std::endl;
                std::cerr << "dx: " << fabs(x.a-x.c) << std::endl;
                if(fabs(f.a-f.c)<=0)
                    return true;
                const T new_width = fabs(x.c-x.a);
                if(new_width>=width)
                    return true;
                width = new_width;
                goto CYCLE;
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zrid);
        };

    }

}

#endif

