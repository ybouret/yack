
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
            typedef solo_repo<const equilibrium> er_repo;

            explicit steady(const reactor    &cs_,
                            writable<double> &K_);

            virtual ~steady() throw();

            const reactor     &cs;      //!< persistent reactor
            writable<double>  &K;       //!< persistent array of constants
            cxx_array<double>  xi;      //!< [L] extents
            cxx_array<double>  sigma;   //!< [N] slope at xi
            cxx_array<bool>    blocked; //!< [L] blocked
            er_repo            running;
            cxx_array<double>  Corg;    //!< [M]
            cxx_array<double>  Cend;    //!< [M]
            cxx_array<double>  Ctry;    //!< [M]
            matrix<double>     Ceq;     //!< [LxM]
            matrix<double>     Phi;     //!< [NxM] gradient at Corg
            matrix<double>     Psi;     //!< [NxM] gradient at Cend
            rmulops            xmul;
            raddops            xadd;

            void run(writable<double> &C, const xmlog &xml);

            double Hamiltonian(const readable<double> &C);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(steady);
            void run(writable<double> &C, const cluster &cc, const xmlog &xml);

        };

    }

}

#endif

