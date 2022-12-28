//! \file

#ifndef YACK_CHEMICAL_NEXUS_INCLUDED
#define YACK_CHEMICAL_NEXUS_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/chem/active.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/chem/nexus/cluster.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // aliases for Nexus
        //
        //______________________________________________________________________
        typedef matrix<int>                 imatrix;    //!< alias
        typedef matrix<unsigned>            umatrix;    //!< alias
        typedef matrix<double>              rmatrix;    //!< alias

        typedef cxx_array<criterion>       criterions; //!< alias
        typedef cxx_array<double>          tableau;    //!< alias
        
        //______________________________________________________________________
        //
        //
        //! nexus
        //
        //______________________________________________________________________
        class nexus
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char * const clid;    //!< [nexus]
            static bool              &verbose; //!< verbosity

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit nexus(const library    &lib,
                           const equilibria &eqs,
                           const double      t);

            //! cleanup
            virtual ~nexus() throw();
            
            
            //! update Kl after K is computed
            /**
             not constant,
             use memory inside mixed equilibria
             */
            void upgrade_lattice(writable<double> &Kl);
            
            //! compute K
            void compute_singles(const double t);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library    &         corelib;  //!< initial library
            const equilibria &         singles;  //!< initial equilibria
            const equilibria           lattice;  //!< lattice of equilibria
            const enode * const        next_en;  //!< firt node of lattice after single
            const active_list::pointer working;  //!< list of active species
            const size_t               M;        //!< corelib.size
            const size_t               N;        //!< singles.size
            const size_t               L;        //!< lattice.size
            const imatrix              Nu;       //!< [NxM] initial topology
            
        private:
            const library     worklib; //!< hard copy of corelib, to build combinations
            
        public:
            //__________________________________________________________________
            //
            // species data
            //__________________________________________________________________
            const criterions crit;      //!< [M] criterions
            
            //__________________________________________________________________
            //
            // singles data
            //__________________________________________________________________
            tableau                 K;       //!< [N] single constants
            const eq_tier::pointer  genus;   //!< classification of singles
            const clusters          wired;   //!< clusters of connex equilibria with their c-laws
            

            //__________________________________________________________________
            //
            // conservation matrix
            //__________________________________________________________________
            const size_t       Nq; //!< number of conservation laws
            const umatrix      Qm; //!< built from each conservation law

            
            //! build detached matrix from given topology
            void build_detached(matrix<bool> &detached, const imatrix &) const;

            //! print human readable status
            void print_detached(const matrix<bool> &detached) const;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(nexus);
            const lockable::scope lockLib;
            const lockable::scope lockEqs;

            void               resolve_wired(const xmlog &);
            void               make_manifold(const xmlog &);
            void               make_manifold_(cluster &sharing, const xmlog &);
            const equilibrium &promote_mixed(const readable<int> &);
            void               conserved_set(const xmlog &);
            void               conserved_set_(cluster &sharing, const xmlog &);
            
        };

    }
}


#endif

