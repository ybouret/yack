//! \file

#ifndef YACK_CHEM_PLEXUS_INCLUDED
#define YACK_CHEM_PLEXUS_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/container/matrix.hpp"
#include "yack/apex.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/data/list/meta.hpp"

#include <cmath>

namespace yack
{
    namespace chemical
    {



        //______________________________________________________________________
        //
        //
        // global definitions
        //
        //______________________________________________________________________
        typedef matrix<double>           rmatrix; //!< real matrix
        typedef matrix<int>              imatrix; //!< integer matrix
        typedef meta_node<const species> anode;   //!< active node
        typedef meta_list<const species> alist;   //!< active list

        //! helper for verbosity
#define YACK_CHEM_PRINTLN(MSG) do { if(plexus::verbose) { std::cerr << MSG << std::endl; } } while(false)

        //______________________________________________________________________
        //
        //
        //! system operation
        //
        //______________________________________________________________________
        class plexus : public ios::vizible
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arrays_of<double>    tableaux;   //!< alias
            typedef tableaux::array_type array_type; //!< alias
            static bool                  verbose;    //!< verbosity
            static const char            clid[];     //!< chemical::plexus
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~plexus() throw();                //!< cleanup
            explicit plexus(library &, equilibria &); //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void    computeK(const double t);                              //!< pre-compute constants
            void    computeGamma(const readable<double> &C)       throw(); //!< evaluate mass action
            void    computeGammaAndPsi(const readable<double> &C) throw(); //!< evaluate mass action and jacobian
            double  computeVariance(const readable<double> &C);            //!< variance to equilibria, uses sc and Ctmp


            //! evolve, Gamma and Psi are ready on return
            void evolve(writable<double> &C);

            //! variance at Ctry = Corg + u * dC
            double operator()(const double u);

            //! solve, Gamma and Psi are ready on return
            void solve(writable<double> &C);

            //! chemical graph
            void gv(const string &filename) const;
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library       &lib;    //!< support library
            const equilibria    &eqs;    //!< support equilibria
            const size_t         N;      //!< equilibria count
            const size_t         M;      //!< species count
            const size_t         A;      //!< active species
            const alist          active; //!< active species list
            
        private:
            tableaux ntab;
            tableaux mtab;
        public:
            array_type                &K;        //!< [N] constants at a given time
            array_type                &Gamma;    //!< [N] Gamma for precomputed K
            array_type                &Corg;     //!< [M] initial concentrations
            array_type                &Ctry;     //!< [M] trial concentrations
            array_type                &Ctmp;     //!< [M] trial concentrations
            array_type                &dC;       //!< [M] delta C
            array_type                &xi;       //!< [N] xi
            array_type                &sc;       //!< [N] to compute variance
            array_type                &xs;       //!< [N] extent coefficients
            const imatrix              Nu;       //!< topology
            const imatrix              NuT;      //!< Nu'
            rmatrix                    Psi;      //!< [NxM] jacobian of Gamm
            rmatrix                    W;        //!< [NxN] Psi*NuT
            vector<bool>               blocked;  //!< stack of blocked equilibria indices
            vector<double>             rstack;   //!< temporary stack of real
            vector<size_t>             ustack;   //!< temporary stack of unsigned
            math::lu<double>           LU;       //!< [N]

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            const lockable::scope lib_lock;
            const lockable::scope eqs_lock;

            void computeConn();

            const readable<double> & make_trial(const double u) throw(); //!< Ctry = Corg + u * dC, >=0
            const readable<double> & make_trial(const double u, const readable<size_t> &vanishing) throw(); //!< Ctry = Corg + u * dC, >=0, set vanishing

            double gammaVariance() throw();
            void   shrink(double &g0);

            //! compute with regularized composition
            void computeExtent();

            //! compute precise deltaC from a given extent
            void computeDeltaC(const readable<double> &x) throw();

            //!  minimize blocked equilibria.
            /**
             - from GammaAndPhi at Corg, move to the most regular concentration
             - return number of moved equilibria
             */
            size_t regularize();
            

            //! detect truncation, use rstack and ustack
            size_t truncation(double &scale);

            //! move algorithm: from g0@Corg to g1@Ctry
            double move(const double g0);

            //! helper
            void save_profile(const char *filename, const double umax);

        };
        
    }
}

#endif

