//! \file

#ifndef YACK_CHEM_VAT_EQUALIZER_INCLUDED
#define YACK_CHEM_VAT_EQUALIZER_INCLUDED 1

#include "yack/chem/vat/cluster.hpp"
#include "yack/chem/vat/metrics.hpp"


namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Concentrations equalizer
        //
        //______________________________________________________________________
        class Equalizer
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Equalizer();
            virtual ~Equalizer() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void run(const xmlog      &xml,
                     writable<double> &C0,
                     const Cluster    &cl);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Species::Fund   speciesFund;
            Cursor::Fund    cursorsFund;
            Metrics         reac;
            Metrics         prod;
            Extended::Adder xadd;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equalizer);
        };

    }

}

#endif
