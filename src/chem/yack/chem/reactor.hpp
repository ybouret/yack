
//! \file

#ifndef YACK_CHEMICAL_REACTOR_INCLUDED
#define YACK_CHEMICAL_REACTOR_INCLUDED 1

#include "yack/chem/active.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/chem/eqs/groups.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
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
            bool solve(writable<double> &C0);

            //__________________________________________________________________
            //
            // member
            //__________________________________________________________________
            const library    &corelib; //!< initial library
            const equilibria &singles; //!< initial equilibria
            const active_list working; //!< list of active species
            const size_t      M;       //!< corelib.size
            const size_t      N;       //!< singles.size
            const size_t      L;       //!< lattice.size
            const equilibria  couples; //!< all composite
            const equilibria  lattice; //!< singles|couples
            const groups      related; //!< related
            rmulops           xmul;    //!< for mulops
            raddops           xadd;    //!< for addops

        private:
            const library     worklib; //!< copy of corelib
            tableaux          mtab;    //!< [M]-sized
            tableaux          ntab;    //!< [N]-sized
            tableaux          ltab;    //!< [L]-sized
            
        public:
            const imatrix      Nu;      //!< [NxM] topology
            rmatrix            Psi;     //!< [NxM] jacobian
            tableau           &Corg;    //!< [M] starting point
            tableau           &Cend;    //!< [M] final point
            tableau           &Ctry;    //!< [M] working space
            tableau           &K;       //!< [N] pre-computed K
            tableau           &sigma;   //!< [N] slope
            tableau           &Kl;      //!< [L] pre-computed K
            tableau           &Xl;      //!< [L] solving extents
            thin_array<bool>   blocked; //!< [L]
            rmatrix            Ceq;     //!< [LxM] all single solutions

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
            const lockable::scope lockLib;
            const lockable::scope lockEqs;

            bool   solved(writable<double> &C0);
            void   build_related();
            void   make_manifold();
        };


    }

}

#endif

