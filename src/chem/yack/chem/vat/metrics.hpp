//! \file

#ifndef YACK_CHEM_VAT_METRICS_INCLUDED
#define YACK_CHEM_VAT_METRICS_INCLUDED 1

#include "yack/chem/vat/regulating.hpp"
#include "yack/chem/vat/equalizing.hpp"


namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Metrics for reactants or products
        //
        //______________________________________________________________________
        class Metrics
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Metrics(const Species::Fund &,
                             const Cursor::Fund  &) noexcept; //!< setup
            virtual ~Metrics() noexcept;                      //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //!
            /**
             \param C0       converted concentrations
             \param actors   reac/prod
             \param category SubLevel category
             */
            void       computeFrom(const readable<Extended::Real>    &C0,
                                   const Actors                      &actors,
                                   const readable<Species::Category> &category,
                                   const IndexLevel                   level);

            //! display
            YACK_PROTO_OSTREAM(Metrics);

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            Regulating regulating; //!< regulating extent
            Equalizing equalizing; //!< equalizing extent(s)

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Metrics);
        };


    }

}

#endif
