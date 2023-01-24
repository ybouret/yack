
//! \file

#ifndef YACK_CHEMICAL_STEADY_INCLUDED
#define YACK_CHEMICAL_STEADY_INCLUDED 1

#include "yack/chem/reactor.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! scene of species and equilibria
        //
        //______________________________________________________________________
        class steady
        {
        public:
            explicit steady(const reactor    &cs_,
                            writable<double> &K_);

            virtual ~steady() throw();

            const reactor     &cs;    //!< persistent reactor
            writable<double>  &K;     //!< persistent array of constants
            cxx_array<double>  xi;    //!< [L] extents
            cxx_array<double>  sigma; //!< [L] slope at xi
            cxx_array<double>  Corg;  //!< [M]
            cxx_array<double>  Cend;  //!< [M]
            cxx_array<double>  Ctry;  //!< [M]
            matrix<double>     Cz;    //!< [LxM]
            rmulops            xmul;
            raddops            xadd;

            void run(writable<double> &C, const xmlog &xml);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(steady);
            void run(writable<double> &C, const cluster &cc, const xmlog &xml);

        };

    }

}

#endif

