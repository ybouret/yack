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

            //! try to adjust conserved species
            /**
             \param xml     display info
             \param C0      [TopLevel] concentrations
             \param cluster cluster to process
             */
            bool runConserved(const xmlog              &xml,
                              writable<Extended::Real> &C0,
                              const Cluster            &cluster);

            //! adjust ubounded species
            /**
             \param xml     display info
             \param C0      [TopLevel] concentrations
             \param cluster cluster to process
             */
            void runUbounded(const xmlog              &xml,
                             writable<Extended::Real> &C0,
                             const Cluster            &cluster);

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
            Species::CoopRepo     nomad;       //!< unbounded

            XVector               Corg;        //!< Extended C0
            XVector               Ctmp;        //!< auxiliary
            XVector               Cend;        //!< auxiliary
            XVector               xbad;        //!< initial bad values
            XMatrix               Ceqz;        //!< equalized
            Extended::Adder       xadd;        //!< internal ops
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equalizer);



            //! compute Ctmp and its gain
            Extended::Real gainOf(const Tribe &, const Cluster &);

        };

    }

}

#endif
