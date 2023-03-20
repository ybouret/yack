
//! \file

#ifndef YACK_AQUEOUS_COLL_CHART_INCLUDED
#define YACK_AQUEOUS_COLL_CHART_INCLUDED 1

#include "yack/aqueous/eqs/frontiers.hpp"
#include "yack/aqueous/realm/coll/caches.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace collecting
        {
            class chart
            {
            public:
                explicit chart(const caches &) noexcept; //!< setup
                virtual ~chart()               noexcept; //!< cleanup
                
                frontiers reac;
                frontiers prod;
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(chart);
            };
        }
    }

}

#endif
