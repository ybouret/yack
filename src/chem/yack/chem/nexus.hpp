//! \file

#ifndef YACK_CHEMICAL_NEXUS_INCLUDED
#define YACK_CHEMICAL_NEXUS_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/chem/active.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/arrays.hpp"

namespace yack
{
    namespace chemical
    {

        typedef matrix<int>                 imatrix;  //!< alias
        typedef arrays_of<double>           tableaux; //!< alias
        typedef tableaux::array_type        tableau;  //!< alias
        typedef thin_array<const criterion> criterions;

        class nexus
        {
        public:
            static const char * const clid;
            static bool              &verbose;

            virtual ~nexus() throw();
            explicit nexus(const library    &lib,
                           const equilibria &eqs,
                           const double      t);

            const library    &  corelib;  //!< initial library
            const equilibria &  singles;  //!< initial equilibria
            const active_list   working;  //!< list of active species
            const size_t        M;        //!< corelib.size
            const size_t        N;        //!< singles.size
            const imatrix       Nu;       //!< [NxM] topology
            

        private:
            const library     worklib;  //!< hard copy of corelib, to build combinations
            tableaux          mtab;    //!< [M]-sized
            tableaux          ntab;    //!< [N]-sized

        public:
            // species data
            const criterions crit;

            // eqs data
            const eq_team    regular; //!< both ways
            const eq_team    roaming; //!< [part|join]_only

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(nexus);
            const lockable::scope lockLib;
            const lockable::scope lockEqs;
        };

    }
}


#endif

