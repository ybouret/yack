//! \file

#ifndef YACK_CHEMICAL_NEXUS_INCLUDED
#define YACK_CHEMICAL_NEXUS_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/chem/active.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/chem/nexus/cluster.hpp"

namespace yack
{
    namespace chemical
    {

        typedef matrix<int>                 imatrix;    //!< alias
        typedef matrix<unsigned>            umatrix;    //!< alias
        typedef matrix<double>              rmatrix;    //!< alias
        
        typedef arrays_of<double>           tableaux;   //!< alias
        typedef tableaux::array_type        tableau;    //!< alias
        typedef thin_array<const criterion> criterions; //!< alias

      
        class conservation_law : public object, public actors
        {
        public:
            explicit conservation_law() throw() : object(), actors(), next(0), prev(0) {}
            virtual ~conservation_law() throw() {}

            friend std::ostream & operator<<(std::ostream &os, const conservation_law &self)
            {
                const actors &me = self;
                os << "0=d(" << me << ")";
                return os;
            }

            conservation_law *next;
            conservation_law *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conservation_law);
        };

        typedef cxx_list_of<conservation_law> conservation_laws;


        //! nexus
        class nexus
        {
        public:
            static const char * const clid;    //!< [nexus]
            static bool              &verbose; //!< verbosity

            //! setup
            explicit nexus(const library    &lib,
                           const equilibria &eqs,
                           const double      t);

            //! cleanup
            virtual ~nexus() throw();

            const library    &  corelib;  //!< initial library
            const equilibria &  singles;  //!< initial equilibria
            const equilibria    lattice;  //!< lattice of equilibria
            const enode * const next_en;  //!< firt node of lattice after single
            const active_list   working;  //!< list of active species
            const size_t        M;        //!< corelib.size
            const size_t        N;        //!< singles.size
            const size_t        L;        //!< lattice.size
            const imatrix       Nu;       //!< [NxM] topology
            raddops             xadd;
            rmulops             xmul;

        private:
            const library     worklib; //!< hard copy of corelib, to build combinations
            tableaux          mtab;    //!< [M]-sized
            tableaux          ntab;    //!< [N]-sized
            tableaux          ltab;    //!< [L]-sized
            
        public:
            // species data
            const criterions crit;      //!< [M] criterions

            // singles data
            tableau           &K;       //!< [N] single constants
            const eq_team      regular; //!< both ways
            const eq_team      roaming; //!< [part|join]_only
            const clusters     related; //!< clusters of related equilibria

            // lattice data
            tableau           &Kl;      //!< [L] constants

            // conservation data
            const size_t             Nc;      //!< number of conservation laws
            const umatrix            Qc;      //!< [Nc*M] matrix of positive coefficients
            const conservation_laws  Ql;      //!< list of Nc laws

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(nexus);
            const lockable::scope lockLib;
            const lockable::scope lockEqs;

            void build_related(const xmlog &);
            void make_manifold(const xmlog &);
            void conserved_set(const xmlog &);
            
        };

    }
}


#endif

