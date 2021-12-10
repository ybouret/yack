//! \file

#ifndef YACK_RANDOMIZED_METRICS_INCLUDED
#define YACK_RANDOMIZED_METRICS_INCLUDED 1

#include "yack/arith/base2.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace randomized
    {

        //______________________________________________________________________
        //
        //
        //! base class for metrics
        //
        //______________________________________________________________________
        class metrics
        {
        public:
            typedef uint32_t       word_type;                     //!< alias
            static const unsigned  word_size = sizeof(word_type); //!< alias
            static const unsigned  word_bits = word_size << 3;    //!< alias
            static const word_type word_unit = 1;                 //!< alias

            explicit metrics(unsigned nbit) throw();
            virtual ~metrics() throw();

            const unsigned  span_bits;
            const word_type span_mask;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(metrics);
        };

        //______________________________________________________________________
        //
        //
        //! built-in limitations
        //
        //______________________________________________________________________
        template <typename T>
        class metrics_of : public metrics
        {
        public:
            static const unsigned   system_bits; //!< from floating point
            static const word_type  system_mask; //!< 2^max_bits-1

            inline  metrics_of(const word_type umax) throw() :
            metrics(min_of<word_type>( bits_for(umax), system_bits))
            {
            }

            inline virtual ~metrics_of() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(metrics_of);
        };

        

    }
}

#endif

