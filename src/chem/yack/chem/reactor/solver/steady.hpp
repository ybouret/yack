
//! \file

#ifndef YACK_CHEMICAL_STEADY_INCLUDED
#define YACK_CHEMICAL_STEADY_INCLUDED 1

#include "yack/chem/reactor.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack {

    namespace chemical {

        //! base type for a local repository of equilibria
        typedef solo_repo<const equilibrium> er_repo_;

        //! local repository with adapted output function
        class er_repo : public er_repo_
        {
        public:
            explicit er_repo(const size_t n) : er_repo_(n) {} //!< setup
            virtual ~er_repo() throw() {}                     //!< cleanup

            YACK_PROTO_OSTREAM(er_repo); //!< display

            bool ratifies(const squad &sq) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(er_repo);
        };

        //______________________________________________________________________
        //
        //
        //! scene of species and equilibria
        //
        //______________________________________________________________________
        class steady
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit steady(const reactor &cs_, writable<double> &K_); //!< setup
            virtual ~steady() throw();                                 //!< cleanup

            const reactor     &cs;      //!< persistent reactor
            const cluster     *cc;      //!< current cluster
            writable<double>  &K;       //!< persistent array of constants
            cxx_array<double>  xi;      //!< [L] extents
            cxx_array<double>  sigma;   //!< [N] slope at xi
            cxx_array<bool>    blocked; //!< [L] blocked
            er_repo            running; //!< [0:N]
            er_repo            solving; //!< [0:L]
            cxx_array<double>  Corg;    //!< [M]
            cxx_array<double>  Cend;    //!< [M]
            cxx_array<double>  Ctry;    //!< [M]
            matrix<double>     Ceq;     //!< [LxM]
            matrix<double>     Phi;     //!< [NxM] gradient at Corg
            matrix<double>     Psi;     //!< [NxM] gradient at Cend
            rmulops            xmul;    //!< internal multiplications
            raddops            xadd;    //!< internal additions

            void   run(writable<double> &C, const xmlog &xml); //!< top level function
            double Hamiltonian(const readable<double> &C);     //!< hamiltonian from running singles
            double optimized_H(const double H0);               //!< from H0=Hamiltonian(Corg) to Cend
            double Hamiltonian(writable<double> &Cout, const squad &sq);
            
            //! Hamiltonian on Corg to Cend
            double operator()(const double u);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(steady);
            void run(writable<double> &C, const cluster &, const xmlog &xml);

        };

    }

}

#endif

