
//! \file

#ifndef YACK_CHEM_VAT_NORMALIZER_INCLUDED
#define YACK_CHEM_VAT_NORMALIZER_INCLUDED 1

#include "yack/chem/vat/custodian.hpp"
#include "yack/chem/vat/equalizer.hpp"

namespace yack
{
    namespace Chemical
    {

        class Normalizer
        {
        public:
            explicit Normalizer(const size_t maxClusterSize,
                                const size_t maximumSpecies);
            virtual ~Normalizer() noexcept;



            void process(const xmlog              &xml,
                         writable<Extended::Real> &C0,
                         const Cluster            &cluster);

            void operator()(const xmlog              &xml,
                            writable<Extended::Real> &C0,
                            const list_of<Cluster>   &clusters);
            

            Custodian  custodian;
            Equalizer  equalizer;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Normalizer);
        };

    }

}

#endif

