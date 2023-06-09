
//! \file

#ifndef YACK_CHEM_VAT_EQUALIZING_INCLUDED
#define YACK_CHEM_VAT_EQUALIZING_INCLUDED 1

#include "yack/chem/vat/equalizer/cursor.hpp"

namespace yack
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! list of equalizing cursors
        //
        //______________________________________________________________________
        class Equalizing : public Cursor::List
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Equalizing(const Species::Fund &,
                                const Cursor::Fund  &) noexcept; //!< setup
            virtual ~Equalizing()                      noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! insert new or update cursor multiplicity
            void tryInsertCursor(const Extended::Real c, const Actor &a);

            //! find best effor regulating limit to improve positivity
            void findBestEffort(Limit &regulating);



            YACK_PROTO_OSTREAM(Equalizing); //!< display
            bool isValid() const noexcept;  //!< to debug

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equalizing);
            Species::Fund speciesFund;
        };

    }

}

#endif

