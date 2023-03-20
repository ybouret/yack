
//! \file

#ifndef YACK_AQUEOUS_BOUNDARIES_INCLUDED
#define YACK_AQUEOUS_BOUNDARIES_INCLUDED 1

#include "yack/aqueous/eqs/zlimit.hpp"
#include "yack/data/dinky/coop-list.hpp"

namespace yack
{
    namespace aqueous
    {
        typedef coop_list<zlimit>       zlimits;
        typedef zlimits::node_type      zl_node;
        typedef zlimits::proxy_type     zl_proxy;
        typedef zlimits::zpool_type     zl_zpool;

        class boundaries : public zlimits 
        {
        public:
            explicit boundaries(const zl_proxy &,
                                const sp_proxy &) noexcept;
            virtual ~boundaries() noexcept;

            void     expand(const double   xi,
                             const species &sp);



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundaries);
            const sp_proxy spore;
        };

    }
}

#endif

