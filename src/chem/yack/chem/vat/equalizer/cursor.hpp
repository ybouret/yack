
//! \file

#ifndef YACK_CHEM_VAT_CURSOR_INCLUDED
#define YACK_CHEM_VAT_CURSOR_INCLUDED 1

#include "yack/chem/eqs/limit.hpp"
#include "yack/data/dinky/coop-list.hpp"
#include "yack/chem/eqs/actor.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Cursor for Equalizer, holding extent and vanishing species
        //
        //______________________________________________________________________
        class Cursor : public Limit
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef coop_list<Cursor> List; //!< alias
            typedef List::node_type   Node; //!< alias
            typedef List::zpool_type  Bank; //!< alias
            typedef List::proxy_type  Fund; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! initialize with first species
            explicit Cursor(const Extended::Real  &c,
                            const Actor           &a,
                            const Species::Fund   &f);

            //! cleanup
            virtual ~Cursor() noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Cursor);
        };

    }

}
#endif

