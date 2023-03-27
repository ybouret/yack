//! \file

#ifndef YACK_AQUEOUS_REALM_NORMALIZER_INCLUDED
#define YACK_AQUEOUS_REALM_NORMALIZER_INCLUDED 1

#include "yack/aqueous/realm/claw/custodian.hpp"
#include "yack/aqueous/realm/collector.hpp"
#include "yack/aqueous/realm.hpp"

namespace yack
{
    namespace aqueous
    {

        class normalizer : public spot_object
        {
        public:

            explicit normalizer(const realm &); //!< setup using realm's metrics
            virtual ~normalizer() noexcept;

            const custodian    guard; //!< guard laws
            const collector    slide; //!< slide equilibria
            cameo::add<double> xadd;  //!< for ops

            void operator()(const realm &, writable<double> &C);
            



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(normalizer);
        };

    }
}

#endif

