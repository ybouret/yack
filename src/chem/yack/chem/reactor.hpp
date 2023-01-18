//! \file

#ifndef YACK_CHEMICAL_REACTOR_INCLUDED
#define YACK_CHEMICAL_REACTOR_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/reactor/types/booleans.hpp"
#include "yack/chem/reactor/cluster.hpp"

namespace yack {

    namespace chemical {

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
            // types and definition
            //__________________________________________________________________
            static const char * const        clid;         //!< "[reactor]"
            static bool                     &verbose;      //!< global
            typedef auto_ptr<const clusters> clusters_ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! full setup
            explicit reactor(library                &lib_,
                             equilibria             &eqs_,
                             const readable<double> &K_);

            //! cleanup
            virtual ~reactor() throw();

            //! maximum size of a group of a conservation laws within a cluster
            size_t max_claws_per_cluster() const throw();

            //! save spectator ions
            void viz_obs(ios::ostream &fp) const;

            //! save all layouts
            void graphViz(const string &root) const;

            //! maximum degree of crossed equilibria
            size_t max_degree() const throw();

            //! maximum actors
            size_t max_actors() const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library          &lib;   //!< library
            const equilibria       &eqs;   //!< singles
            const equilibria        all;   //!< lattice
            const readable<double> &K;     //!< user's memory for K
            const alist::ptr        act;    //!< active species, compress indices
            const auto_ptr<sp_repo> obs;    //!< spectator species
            const size_t            M;      //!< total species
            const size_t            N;      //!< number of single
            const matrix<int>       Nu;     //!< global topology
            const booleans::ptr     fixed;  //!< bounded species, global
            const clusters_ptr      linked; //!< clusters of linked equilibria
            const size_t            Nq;     //!< number of conservations
            const matrix<unsigned>  Qm;     //!< [Nq:M] matrix of conservations
            const size_t            L;      //!< lattice size = all.size()
            const enode * const     el;     //!< first node of mixed equilibria
            
        private:
            const lockable::scope libLock;
            const lockable::scope eqsLock;
            
            void build_related(const xmlog &); //!< build related clusters in linked
            void compile_claws(const xmlog &); //!< compile claws
            void setup_lattice(const xmlog &);
            void create_squads(const xmlog &);
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
        };

    }
}

#endif

