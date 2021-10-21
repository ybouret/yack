
//! \file

#ifndef YACK_RANDOMIZED_UBITS_INCLUDED
#define YACK_RANDOMIZED_UBITS_INCLUDED 1

#include "yack/randomized/bits.hpp"

namespace yack
{

    namespace randomized
    {

        //______________________________________________________________________
        //
        //
        //! bits with a given working word
        //
        //______________________________________________________________________
        template <typename T>
        class bits_on : public bits
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef T          word_type; //!< alias
            typedef bits_on<T> self_type; //!< alias

            //__________________________________________________________________
            //
            //! generation from metrics
            //__________________________________________________________________
            virtual double operator()(void) throw()
            {
                const T nxt = next();
                assert(nxt<=numerator);
                return (0.5+double(nxt))/denominator;
            }

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~bits_on() throw() {} //!< cleanup

        protected:
            //! setup metrics
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
