//! \file

#ifndef YACK_WOVEN_METRICS_INCLUDED
#define YACK_WOVEN_METRICS_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace woven
    {
        class metrics
        {
        public:
            explicit metrics(const size_t dims);
            virtual ~metrics() noexcept;
            metrics(const metrics &) noexcept;

            const size_t dimensions;

        private:
            YACK_DISABLE_ASSIGN(metrics);
        };
    }
}

#endif

