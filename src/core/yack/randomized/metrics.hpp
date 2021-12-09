//! \file

#ifndef YACK_RANDOMIZED_METRICS_INCLUDED
#define YACK_RANDOMIZED_METRICS_INCLUDED 1

#include "yack/setup.hpp"
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

            explicit metrics();
            virtual ~metrics() throw();

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
            static const unsigned   max_bits; //!< from real mantissa
            static const word_type  max_mask; //!< 2^max_bits-1

            inline  metrics_of() throw() : metrics() {}
            inline ~metrics_of() throw() {}

            static inline T word_to_unit(const word_type w) throw()
            {
                static const T half(0.5);
                static const T uden = T(max_mask)  + T(1);
                return (static_cast<T>(w&max_mask) + half)/uden;
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(metrics_of);
        };

        //! declaring constants if necessary
#define YACK_RANDOMIZED_METRICS(VALUE)                           \
template <> const float       metrics_of<float>      ::VALUE; \
template <> const double      metrics_of<double>     ::VALUE; \
template <> const long double metrics_of<long double>::VALUE

        //YACK_RANDOMIZED_METRICS(unit_den);     //!< unit_den


    }
}

#endif

