//! \file

#ifndef YACK_WOVEN_METRICS_INCLUDED
#define YACK_WOVEN_METRICS_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace woven
    {
        //______________________________________________________________________
        //
        //
        //! common metrics for derived class
        //
        //______________________________________________________________________
        class metrics
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit metrics(const size_t dims); //!< check dims >= 2
            virtual ~metrics() noexcept;         //!< cleanup
            metrics(const metrics &) noexcept;   //!< copy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t dimensions; //!< phase space dimensions
            
        private:
            YACK_DISABLE_ASSIGN(metrics);
        };
    }
}

#endif

