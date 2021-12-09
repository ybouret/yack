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
            static const unsigned   system_bits; //!< from floating point
            static const word_type  system_mask; //!< 2^max_bits-1

            inline  metrics_of(const word_type  ) throw() : metrics()
            {

            }

            inline ~metrics_of() throw() {}



            const unsigned usr_bits;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(metrics_of);
        };

        //! declaring constants if necessary
#define YACK_RANDOMIZED_METRICS(VALUE)                        \
template <> const float       metrics_of<float>      ::VALUE; \
template <> const double      metrics_of<double>     ::VALUE; \
template <> const long double metrics_of<long double>::VALUE

        //YACK_RANDOMIZED_METRICS(unit_den);     //!< unit_den

        class converter
        {
        public:

            explicit converter(const uint32_t umax) throw();
            virtual ~converter() throw();
            


        private:

        };


    }
}

#endif

