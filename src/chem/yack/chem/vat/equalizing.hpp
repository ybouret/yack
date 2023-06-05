
//! \file

#ifndef YACK_CHEM_VAT_EQUALIZING_INCLUDED
#define YACK_CHEM_VAT_EQUALIZING_INCLUDED 1

#include "yack/chem/vat/cursor.hpp"

namespace yack
{
    namespace Chemical
    {

        //! list of equalizing cursors
        class Equalizing : public Cursor::List
        {
        public:
            explicit Equalizing(const Species::Fund &,
                                const Cursor::Fund  &) noexcept;
            virtual ~Equalizing()                      noexcept;

            void tryInsertCursor(const double c, const Actor &a);
            YACK_PROTO_OSTREAM(Equalizing);

            bool isValid() const noexcept;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equalizing);
            Species::Fund speciesFund;
        };

    }

}

#endif

