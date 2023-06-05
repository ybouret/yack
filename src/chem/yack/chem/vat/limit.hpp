
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
            explicit Limit(const Species::Fund &) noexcept;
            virtual ~Limit() throw();
            Limit(const Limit &);
            Limit & operator=(const Limit &);
            virtual void initialize() noexcept;

            YACK_PROTO_OSTREAM(Limit);

            
        };

    }

}

#endif

