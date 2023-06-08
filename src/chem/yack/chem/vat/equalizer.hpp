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
            typedef vector<Extended::Real,memory::dyadic> XVector;
            typedef matrix<Extended::Real>                XMatrix;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Equalizer(const size_t maxClusterSize,
                               const size_t maximumSpecies); //!< setup
            virtual ~Equalizer() noexcept;     //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! best effort equalization
            void run(const xmlog      &xml,
                     writable<double> &C0,
                     const Cluster    &cl);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Extended::Real  _0;
            Species::Fund         speciesFund; //!< shared memory for species
            Cursor::Fund          cursorsFund; //!< shared memory for cursors
            Metrics               reac;        //!< metrics for reactants
            Metrics               prod;        //!< metrics for products

            Equilibrium::SoloRepo inUse;       //!< used equilibria
            Species::CoopRepo     valid;       //!< valid species concentration
            Species::CoopRepo     wrong;       //!< wrong species concentration
            Species::CoopRepo     nomad;       //!< unbouded

            XVector               Corg;        //!< Extended C0
            XVector               Ctmp;        //!< auxiliary
            XVector               Cend;        //!< auxiliary
            XVector               xbad;        //!< initial bad values
            XMatrix               Ceqz;        //!< equalized
            Extended::Adder       xadd;        //!< internal ops
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equalizer);

            void runConserved(const xmlog      &xml,
                              writable<double> &C0,
                              const Cluster    &cl);

            //! compute Ctmp and its gain
            Extended::Real gainOf(const Tribe &, const Cluster &);

        };

    }

}

#endif
