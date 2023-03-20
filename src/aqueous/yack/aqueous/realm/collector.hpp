
//! \file

#ifndef YACK_AQUEOUS_COLLECTOR_INCLUDED
#define YACK_AQUEOUS_COLLECTOR_INCLUDED 1

#include "yack/aqueous/realm/domain.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/aqueous/eqs/boundaries.hpp"

namespace yack
{
    namespace aqueous
    {
        

        class collector
        {
        public:

            //! 
            explicit collector(const size_t n) :
            zl_cache( new zl_zpool() ),
            sp_cache( new sp_zpool() )
            {
            }

            virtual ~collector() noexcept {}
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(collector);
            zl_proxy zl_cache;
            sp_proxy sp_cache;
        };

    }

}

#endif

