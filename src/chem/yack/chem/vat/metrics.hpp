//! \file

#ifndef YACK_CHEM_VAT_METRICS_INCLUDED
#define YACK_CHEM_VAT_METRICS_INCLUDED 1

#include "yack/chem/vat/regulating.hpp"
#include "yack/chem/vat/equalizing.hpp"


namespace yack
{
    namespace Chemical
    {

        //! Metrics for reactants or products
        class Metrics
        {
        public:
            explicit Metrics(const Species::Fund &,
                             const Cursor::Fund  &) noexcept;
            virtual ~Metrics() noexcept;

            Regulating regulating;
            Equalizing equalizing;

            //!
            /**
             \param C0       TopLevel concentrations
             \param actors   reac/prod
             \param category SubLevel category
             */
            void       computeFrom(const readable<double>            &C0,
                                   const Actors                      &actors,
                                   const readable<Species::Category> &category);

            YACK_PROTO_OSTREAM(Metrics);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Metrics);
        };


    }

}

#endif
