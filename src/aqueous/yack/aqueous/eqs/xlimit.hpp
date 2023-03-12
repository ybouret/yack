
//! \file

#ifndef YACK_AQUEOUS_XLIMIT_INCLUDED
#define YACK_AQUEOUS_XLIMIT_INCLUDED 1

#include "yack/aqueous/eqs/actors.hpp"
#include "yack/data/dinky/coop-repo.hpp"

namespace yack
{
    namespace aqueous
    {
        typedef coop_repo<const species> sp_repo;
        typedef sp_repo::zpool_type      sp_zpool;
        typedef sp_repo::proxy_type      sp_proxy;

        class xlimit : public sp_repo
        {
        public:
            explicit xlimit(const sp_proxy &) noexcept;
            virtual ~xlimit() noexcept;
            double   xi;

            void     initialize() noexcept;
            void     get_extent(const actors           &A,
                                const readable<double> &C);

            //void     get_demand();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimit);
        };

    }

}

#endif


