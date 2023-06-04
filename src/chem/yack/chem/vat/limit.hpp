
//! \file

#ifndef YACK_CHEM_VAT_LIMIT_INCLUDED
#define YACK_CHEM_VAT_LIMIT_INCLUDED 1

#include "yack/chem/vat/cluster.hpp"
#include "yack/data/dinky/coop-list.hpp"

namespace yack
{
    namespace Chemical
    {

        // Limit: value and vanishing species
        class Limit : public Extended::Real, public Species::CoopRepo
        {
        public:
            virtual ~Limit() throw();
            virtual void initialize() noexcept;

            YACK_PROTO_OSTREAM(Limit);

        protected:
            explicit Limit(const Species::Fund &) noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Limit);
        };

    }

}

#endif

