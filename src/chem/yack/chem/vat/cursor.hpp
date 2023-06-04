
//! \file

#ifndef YACK_CHEM_VAT_CURSOR_INCLUDED
#define YACK_CHEM_VAT_CURSOR_INCLUDED 1

#include "yack/chem/vat/limit.hpp"
#include "yack/data/dinky/coop-list.hpp"

namespace yack
{
    namespace Chemical
    {

        //! cursor for equalizer
        class Cursor : public Limit
        {
        public:
            typedef coop_list<Cursor> List;
            typedef List::node_type   Node;
            typedef List::zpool_type  Bank;
            typedef List::proxy_type  Fund;

            //! initialize with first species
            explicit Cursor(const double         c,
                            const Actor         &a,
                            const Species::Fund &f);

            //! cleanup
            virtual ~Cursor() noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Cursor);
        };

    }

}
#endif

