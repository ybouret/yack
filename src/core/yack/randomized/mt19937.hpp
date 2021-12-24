//! \file

#ifndef YACK_RANDOMIZED_MT19937_INCLUDED
#define YACK_RANDOMIZED_MT19937_INCLUDED 1

#include "yack/randomized/bits.hpp"

namespace yack
{

    namespace randomized
    {

        //______________________________________________________________________
        //
        //
        //! Mersenne Twister
        //
        //______________________________________________________________________
        class mt19937 : public bits
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const size_t N          = 624;          //!< period parameter
            static const size_t M          = 397;          //!< period parameter
            static const long   MATRIX_A   = 0x9908b0dfUL; //!< constant vector a
            static const long   UPPER_MASK = 0x80000000UL; //!< most significant w-r bits
            static const long   LOWER_MASK = 0x7fffffffUL; //!< least significant r bits

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~mt19937() throw();                           //!< cleanup
            explicit mt19937(const unsigned long seed=0) throw(); //!< setup
            explicit mt19937(const unsigned long init_key[], const size_t key_length) throw(); //!< setup

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual uint32_t next32() throw(); //!< next word

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! init with any seed
            void init_genrand(unsigned long s) throw();

            //! init with and array of seeds
            void init_by_array(const unsigned long init_key[], const size_t key_length) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mt19937);
            size_t        mti;   //!< initially N+1
            unsigned long mt[N]; //!< state


        };

    }

}

#endif
