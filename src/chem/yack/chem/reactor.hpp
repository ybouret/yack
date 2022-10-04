
//! \file

#ifndef YACK_CHEMICAL_REACTOR_INCLUDED
#define YACK_CHEMICAL_REACTOR_INCLUDED 1

#include "yack/chem/active.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/chem/eqs/groups.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/math/algebra/crout.hpp"

namespace yack
{
    class xmlog;

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        // aliases
        //
        //______________________________________________________________________
        typedef arrays_of<double>    tableaux; //!< alias
        typedef tableaux::array_type tableau;  //!< alias
        typedef matrix<int>          imatrix;  //!< alias
        typedef matrix<double>       rmatrix;  //!< alias
        class outcome;

        //______________________________________________________________________
        //
        //
        //! reactor
        //
        //______________________________________________________________________
        class reactor
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[];  //!< "chemical::reactor"
            static bool      &verbose; //!< entity::verbose
            //!
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~reactor() throw(); //!< cleanup

            explicit reactor(const library    &,
                             const equilibria &,
                             const double     t); //!< setup


            //! best effort solving
            bool   solve(writable<double> &C0);

            //! wrapper Hamiltonian(Corg+u*(Cend-Corg)) with full clipping
            double operator()(const double u);

            //__________________________________________________________________
            //
            // member
            //__________________________________________________________________
            const library    &  corelib;  //!< initial library
            const equilibria &  singles;  //!< initial equilibria
            const active_list   working;  //!< list of active species
            const size_t        M;        //!< corelib.size
            const size_t        N;        //!< singles.size
            const size_t        L;        //!< lattice.size
            const equilibria    couples;  //!< all composite
            const equilibria    lattice;  //!< singles|couples
            const groups        related;  //!< related
            const groups        solving;  //!< all solving combinations
            rmulops             xmul;     //!< for mulops
            raddops             xadd;     //!< for addops
            math::crout<double> solv;     //!< for linear system solving

        private:
            const library     worklib; //!< copy of corelib
            tableaux          mtab;    //!< [M]-sized
            tableaux          ntab;    //!< [N]-sized
            tableaux          ltab;    //!< [L]-sized
            
        public:
            const imatrix      Nu;      //!< [NxM] topology
            imatrix            NuA;     //!< [NXM] topology of non-blocked equilibria
            rmatrix            Psi;     //!< [NxM] jacobian
            rmatrix            Omega;   //!< [NxN] coupling
            rmatrix            iOmega;  //!< [NxN] inv(Omega)
            tableau           &Corg;    //!< [M] starting point
            tableau           &Cend;    //!< [M] final point
            tableau           &Ctry;    //!< [M] working space
            tableau           &dC;      //!< [M] step
            tableau           &K;       //!< [N] pre-computed K
            tableau           &Gamma;   //!< [N] current Gamm
            tableau           &xi;      //!< [N] extent
            tableau           &sigma;   //!< [N] slope
            tableau           &Kl;      //!< [L] pre-computed K
            tableau           &Xl;      //!< [L] solving extents
            thin_array<bool>   blocked; //!< [L] blocking state
            rmatrix            Ceq;     //!< [LxM] all single solutions
            vector<double>     ratio;   //!< [0..M] ratio
        
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
            const lockable::scope lockLib;
            const lockable::scope lockEqs;

            bool               returnSolved(writable<double> &C0, const xmlog&); //!< upon successful return
            void               buildRelated();                           //!< build global partitions
            void               makeManifold(const xmlog &);              //!< build all solving partitions

            const equilibrium *getTopology(size_t &nrun, outcome &);     //!< full topology at Corg
            const equilibrium *getDominant(double &Hmin);                //!< starting from Hmin = H0
            double             Hamiltonian(const readable<double> &C);   //!< a molar hamiltonian
            double             Optimized1D(const double H0);             //!< optimize from (Corg,H0) to Cend
            void               deactivated(const size_t ei);             //!< set deactivated ei
            void               createOmega();                            //!< build Omega and rhs
            double             updateOmega();                            //!< update Omega and rhs from new blocked, return new H0
        };


    }

}

#endif

