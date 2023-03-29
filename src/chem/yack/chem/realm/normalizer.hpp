//! \file

#ifndef YACK_CHEMICAL_REALM_NORMALIZER_INCLUDED
#define YACK_CHEMICAL_REALM_NORMALIZER_INCLUDED 1

#include "yack/chem/realm/claw/custodian.hpp"
#include "yack/chem/realm/collector.hpp"
#include "yack/chem/realm.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! normalizer for a given concentration
        //
        //______________________________________________________________________
        class normalizer : public spot_object
        {
        public:

            explicit normalizer(const realm &); //!< setup using realm's metrics
            virtual ~normalizer() noexcept;

            const custodian    guard; //!< guard laws,       content = last injected
            const collector    slide; //!< slide equilibria, content = last error
            
            void operator()(const realm &, writable<double> &C);
            



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(normalizer);
        };

    }
}

#endif

