//! \file

#ifndef YACK_RANDOMIZED_METRICS_INCLUDED
#define YACK_RANDOMIZED_METRICS_INCLUDED 1

#include "yack/setup.hpp"
namespace yack
{
    namespace randomized
    {

        class metrics
        {
        public:
            typedef uint32_t      word_type;
            static const unsigned word_size = sizeof(word_type);
            static const unsigned word_bits = word_size << 3;

            metrics();
            virtual ~metrics() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(metrics);
        };

        template <typename T>
        class metrics_of : public metrics
        {
        public:

            static const unsigned real_bits; 

            inline  metrics_of() throw() : metrics() {}
            inline ~metrics_of() throw() {}


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(metrics_of);
        };


    }
}

#endif

