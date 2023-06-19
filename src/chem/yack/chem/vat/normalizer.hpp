
//! \file

#ifndef YACK_CHEM_VAT_NORMALIZER_INCLUDED
#define YACK_CHEM_VAT_NORMALIZER_INCLUDED 1

#include "yack/chem/vat/custodian.hpp"
#include "yack/chem/vat/equalizer.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Concentrations normalizer
        //
        //______________________________________________________________________
        class Normalizer
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup with pre-allocated memory
            explicit Normalizer(const size_t maxClusterSize,
                                const size_t maximumSpecies);

            //! cleanup
            virtual ~Normalizer() noexcept;


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! processing a cluster
            /**
             - try to equalize conserved
             - enforce and loop try again if not possible
             - and equalize unbounded
             */
            void process(const xmlog              &xml,
                         writable<Extended::Real> &C0,
                         const Cluster            &cluster);

            //! process a list of clusters
            /**
             - restart custodian
             - process all clusters
             */
            void operator()(const xmlog              &xml,
                            writable<Extended::Real> &C0,
                            const list_of<Cluster>   &clusters);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Custodian  custodian; //!< custodian
            Equalizer  equalizer; //!< equalizer

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Normalizer);
        };

    }

}

#endif

