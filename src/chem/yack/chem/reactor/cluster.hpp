//! \file

#ifndef YACK_CHEMICAL_CLUSTER_INCLUDED
#define YACK_CHEMICAL_CLUSTER_INCLUDED 1

#include "yack/chem/reactor/equilibrium/tier.hpp"
#include "yack/chem/reactor/equilibrium/ledger.hpp"
#include "yack/chem/reactor/equilibrium/squads.hpp"
#include "yack/chem/reactor/species/tier.hpp"
#include "yack/chem/reactor/conservation/groups.hpp"
#include "yack/chem/reactor/types/udict.hpp"
#include "yack/chem/reactor/types/booleans.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack {
    
    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! scene of species and equilibria
        //
        //______________________________________________________________________
        class scene : public object
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const scene> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit scene() : breed( new sp_tier() ), genus( new eq_tier() ) {} //!< setup
            virtual ~scene() noexcept {} //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! specific display
            void display(const xmlog &xml, const char * const pfx) const {
                YACK_XMLSUB(xml,"cluster::scene");
                breed->display(xml,pfx);
                genus->display(xml,pfx);
            }

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            const sp_tier::ptr    breed;  //!< category for species
            const eq_tier::ptr    genus;  //!< category for equilibria

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scene);
        };

        //______________________________________________________________________
        //
        //
        //! cluster of linked species and equilibria
        //
        //______________________________________________________________________
        class cluster : public large_object, public latch
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid; //!< "chemical::cluster"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cluster(const equilibrium   &first,
                             const booleans::ptr &flags); //!< setup with first equilibrium and fixed flags
            virtual ~cluster() noexcept;                  //!< cleanup

            //__________________________________________________________________
            //
            // build methods
            //__________________________________________________________________
            bool  owns(const equilibrium &) const noexcept; //!< mostly to debug
            void  grow(const equilibrium &);               //!< append to group

            //! check if is linked with another equilibrium using global matrix
            bool  linked_with(const equilibrium  &,
                              const matrix<bool> &) const noexcept;

            //! display
            friend std::ostream & operator<<( std::ostream & , const cluster & );

            //! compile once built
            /**
             - collect_alive() : alive from group
             - ignite_system() : init PRIMARY breed and genus
             - conservations() : deduce conservation laws
             */
            void compile(const xmlog            &xml,
                         const matrix<int>      &Nu,
                         const readable<double> &K,
                         const library          &lib,
                         const equilibria       &eqs,
                         equilibria             &all,
                         const unsigned          igv);
            
            //! convert group into simple repo
            void assemble_all_group(eq_repo_ &assembly) const;

            

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void viz(ios::ostream &fp) const; //!< add to a graphViz file
            
            //__________________________________________________________________
            //
            // member
            //__________________________________________________________________
            cluster              *next;   //!< for clusters
            cluster              *prev;   //!< for list

            //------------------------------------------------------------------
            // describing species and equilibria
            //------------------------------------------------------------------
            const alist::ptr      alive;   //!< alive.size, from the begining
            const booleans::ptr   fixed;   //!< true if conserved
            const auto_ptr<glist> group;   //!< group.size, initial+manifold
            const eq_repo::ptr    single;  //!< initial  equilibria
            const eq_repo::ptr    hybrid;  //!< manifold only
            const scene::ptr      primary; //!< primary scene
            const scene::ptr      replica; //!< replica scene
            const udict::ptr      sDict;   //!< LUTs for species, from alive
            const udict::ptr      eDict;   //!< LUTs for equilibria, from group


            //------------------------------------------------------------------
            // describing conservation laws
            //------------------------------------------------------------------
            const claws::ptr      canon;  //!< conservation laws to follow
            const clots::ptr      claim;  //!< groups of independent laws, for custodian

            //------------------------------------------------------------------
            // describing detached equilibria
            //------------------------------------------------------------------
            const squads::ptr     army; //!< all detached for solving
            const squads::ptr     wing; //!< all detached for balancing

            //------------------------------------------------------------------
            // misceallenous
            //------------------------------------------------------------------
            const ledger::ptr     cross;  //!< number of cross/mixed equilibria per degree from manifold
            const unsigned        gvidx;  //!< graphivz index [0:...]
            const unsigned        omega;  //!< omega index = gvidx+1
                                        
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
            std::ostream &      display(std::ostream &) const;
            void                collect_alive();
            void                ignite_system();
            void                conservations(const xmlog &xml);

            void                make_manifold(const xmlog            &xml,
                                              const matrix<int>      &Nu,
                                              const readable<double> &K,
                                              const library          &lib,
                                              const equilibria       &eqs,
                                              equilibria             &all);

            const equilibrium & promote_mixed(const readable<int>    &weight,
                                              const readable<int>    &stoich,
                                              const readable<double> &K,
                                              const library          &lib,
                                              const equilibria       &eqs,
                                              equilibria             &all);

            void                consolidating(const xmlog &xml);

            
        };


        //______________________________________________________________________
        //
        //
        //! list of independent clusters
        //
        //______________________________________________________________________
        class clusters : public object, public cxx_list_of<cluster>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit clusters() noexcept; //!< setup
            virtual ~clusters() noexcept; //!< cleanup
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(clusters);
        };
        
    }
    
}

#endif

