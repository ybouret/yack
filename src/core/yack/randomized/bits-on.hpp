
//! \file

#ifndef YACK_RANDOMIZED_UBITS_INCLUDED
#define YACK_RANDOMIZED_UBITS_INCLUDED 1

#include "yack/randomized/bits.hpp"

namespace yack
{

    namespace randomized
    {

        template <typename T>
        class bits_on : public bits
        {
        public:
            typedef bits_on<T> self_type;

            virtual ~bits_on() throw() {}

            virtual double operator()(void) throw()
            {
                const T nxt = next();
                assert(nxt<=numerator);
                return (0.5+double(nxt))/denominator;
            }


        protected:
            explicit bits_on(const T umax) throw() :
            bits( bits_for(umax) ),
            numerator(umax),
            denominator( 1.0 + double(numerator) )
            {
            }

        public:
            const double  numerator;   //!< unsigned 0..numerator
            const double  denominator; //!< 1.0 + numerator

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bits_on);
            virtual T next() throw() = 0;
        };

    }

}

#endif
