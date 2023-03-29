//! \file

#ifndef YACK_CHEMICAL_COLL_CACHES_INCLUDED
#define YACK_CHEMICAL_COLL_CACHES_INCLUDED 1

#include "yack/chem/eqs/boundaries.hpp"
#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
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
                const eq_proxy eqp; //!< EQuilibria Proxy

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(caches);
            };
        }

    }

}

#endif
