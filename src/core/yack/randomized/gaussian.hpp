//! \file

#ifndef YACK_RANDOMIZED_GAUSSIAN_INCLUDED
#define YACK_RANDOMIZED_GAUSSIAN_INCLUDED 1

#include "yack/randomized/bits.hpp"
#include <cmath>

namespace yack
{
    namespace randomized
    {
        //______________________________________________________________________
        //
        //
        //! gaussian generator using Box-Muller's transform
        //
        //______________________________________________________________________
        template <typename T>
        class gaussian : shared_bits
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~gaussian() throw() {} //!< cleanup
            
            //! setup
            explicit gaussian(const shared_bits &sh) throw() :
            shared_bits(sh),
            gen(true),
            g1(0),
            g2(0)
            {
            }
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! return generated gaussian value
            inline T operator()(void) throw()
            {
                if(gen)
                {
                    generate();
                    return g1;
                }
                else
                {
                    gen = true;
                    return g2;
                }
            }
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(gaussian);
            bool gen;
            T    g1;
            T    g2;
            
            inline void generate() throw()
            {
                static const T    minus_one = T(-1);
                static const T    pi        = acos(minus_one);
                static const T    two_pi    = pi+pi;
                randomized::bits &ran       = **this;
                assert(gen==true);
                const T U     = ran.to<T>();
                const T V     = ran.to<T>();
                const T theta = two_pi * V;
                const T lnU   = log(U);
                const T rho   = sqrt( -(lnU+lnU) );
                g1  = rho * cos(theta);
                g2  = rho * sin(theta);
                gen = false;
            }
        };

    }
}

#endif


