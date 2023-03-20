//! \file

#ifndef YACK_AQUEOUS_COLL_CACHES_INCLUDED
#define YACK_AQUEOUS_COLL_CACHES_INCLUDED 1

#include "yack/aqueous/eqs/boundaries.hpp"

namespace yack
{
    namespace aqueous
    {

        namespace collecting
        {
            //__________________________________________________________________
            //
            //
            //! shared caches for collector
            //
            //__________________________________________________________________
            class caches
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit caches();           //!< create caches
                virtual ~caches() noexcept;  //!< cleanup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const zl_proxy zlp; //!< ZLimit  Proxy
                const sp_proxy spp; //!< SPecies Proxy

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(caches);
            };
        }

    }

}

#endif
