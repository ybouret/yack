
//! \file

#ifndef YACK_CHEMICAL_STEADY_INCLUDED
#define YACK_CHEMICAL_STEADY_INCLUDED 1

#include "yack/chem/reactor.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/data/dinky/solo-repo.hpp"
#include "yack/math/algebra/crout.hpp"

namespace yack {

    namespace chemical {

        //! base type for a local repository of equilibria
        typedef solo_repo<const equilibrium> er_repo_;

        //! local repository with adapted output function
        class er_repo : public er_repo_
        {
        public:
            explicit er_repo(const size_t n) : er_repo_(n) {} //!< setup
            virtual ~er_repo() noexcept {}                     //!< cleanup

            YACK_PROTO_OSTREAM(er_repo); //!< display

            //! check that all the squad is in the repository
            bool ratifies(const squad &sq) const noexcept;

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
            typedef cxx_array<double>            tableau;  //!< alias
            typedef cxx_series< matrix<double> > matrices; //!< alias
            typedef cxx_series<tableau>          tableaux; //!< alias
            typedef math::crout<double>          lu_type;  //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit steady(const reactor &cs_, writable<double> &K_); //!< setup
            virtual ~steady() noexcept;                                 //!< cleanup

            const reactor      &cs;      //!< persistent reactor
            const cluster      *cc;      //!< current cluster
            writable<double>   &K;       //!< persistent array of constants
            tableau             xi;      //!< [L] extents solving each eq
            tableau             sigma;   //!< [N] slope at xi
            tableau             xstep;   //!< [N] solving extent
            cxx_array<bool>     blocked; //!< [L] blocked
            er_repo             running; //!< [0:N]
            er_repo             solving; //!< [0:L]
            tableau             Corg;    //!< [M]
            tableau             Cend;    //!< [M]
            tableau             Ctry;    //!< [M]
            tableau             Phi;     //!< [M]
            matrix<double>      Ceq;     //!< [LxM]
            matrix<double>      Psi;     //!< [NxM] gradient at Cend
            cxx_array<greatest> gPsi;    //!< [N]
            matrices            Omega;   //!< one matrix per cluster
            tableaux            Steps;   //!< one tab per cluster
            rmulops             xmul;    //!< internal multiplications
            raddops             xadd;    //!< internal additions
            auto_ptr<lu_type>   lu;      //!< internal solver
            
            
            void               run(writable<double> &C, const xmlog &xml);            //!< top level function
            double             Hamiltonian(const readable<double> &C);                //!< hamiltonian from running singles
            double             optimized_H(const double H0);                          //!< from H0=Hamiltonian(Corg) to Cend
            double             Hamiltonian(writable<double> &Cout, const squad &sq);  //!< build composite concentration and H
      
            //! check if still running amongst single
            /**
             - look for an unsteady equilibrium with get_running
             - return false if all are OK
             - return true if something to do,  with set_scaling
             - when true, then the global hamiltonian is ready to be computed
             */
            bool               has_running(writable<double> &C, const unsigned, const xmlog &);
         
            //! fill currently running single equilibri[um|a]
            /**
             - check if each equilibrium is running or blocked, filling Ceq
             - updated blocked and xi
             - return the most unsteady equilibrium if found
             */
            const equilibrium *get_running(const readable<double> &C, const xmlog &);
            
            //! compute scaling for each (running) single
            /**
             - if blocked, set sigma to 0
             - compute gradient of mass action at equilibrium
             - compute dot product with stoichiometric coeff
             */
            void               set_scaling(const xmlog &);
           
            //! look for globally solving equilibri[um|a]
            /**
             - for each single, compute the best concentration within C0 -> Ceq
             - for each hybrid, compute its Ceq then the best concentration within C0 -> Ceq
             - register in solving all the improving concentrations
             */
            bool               got_solving(const double H0, const xmlog &);
            
            //! make the best new global concentration from got_solving
            /**
             - test all ratified squads and keep the best
             - compose the new concentration from the best squad
             */
            void               make_global(writable<double> &C, const xmlog &);
            
            
            bool               build_omega(const readable<double> &C, const xmlog &);

            //! Hamiltonian on Corg to Cend
            double operator()(const double u);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(steady);
            void run(writable<double> &C, const cluster &, const xmlog &xml);

        };

    }

}

#endif

