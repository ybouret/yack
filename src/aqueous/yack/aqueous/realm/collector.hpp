
//! \file

#ifndef YACK_AQUEOUS_COLLECTOR_INCLUDED
#define YACK_AQUEOUS_COLLECTOR_INCLUDED 1

#include "yack/aqueous/realm/domain.hpp"
#include "yack/aqueous/eqs/frontiers.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace aqueous
    {

        //______________________________________________________________________
        //
        //
        //! collector for invalid concentrations
        //
        //______________________________________________________________________
        class collector
        {
        public:
            class chart
            {
            public:

                explicit chart(const zl_proxy &z, const sp_proxy &s) noexcept :
                reac(z,s),
                prod(z,s)
                {
                }

                virtual ~chart() noexcept {}


                frontiers reac;
                frontiers prod;
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(chart);
            };

            //!  setup
            explicit collector(const size_t n) :
            zlp( new zl_zpool() ),
            spp( new sp_zpool() )
            {
            }

            //! cleanup
            virtual ~collector() noexcept {}
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(collector);
            zl_proxy zlp;
            sp_proxy spp;
        };

    }

}

#endif

