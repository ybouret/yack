//! \file

#ifndef YACK_CHEMICAL_CLUSTER_INCLUDED
#define YACK_CHEMICAL_CLUSTER_INCLUDED 1

#include "yack/chem/reactor/eq-tier.hpp"
#include "yack/chem/reactor/sp-tier.hpp"
#include "yack/chem/reactor/udict.hpp"
#include "yack/chem/reactor/cl-groups.hpp"
#include "yack/chem/reactor/gvector.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"

namespace yack {
    
    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! cluster of linked species and equilibria
        //
        //______________________________________________________________________
        class cluster : public large_object, public latch
        {
        public:
            static const char * const clid; //!< "chemical::cluster"
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cluster(const equilibrium &first); //!< setup with first equilibrium
            virtual ~cluster() throw();                 //!< cleanup

            //__________________________________________________________________
            //
            // build methods
            //__________________________________________________________________
            bool  owns(const equilibrium &) const throw(); //!< mostly to debug
            void  grow(const equilibrium &);               //!< append to group

            //! check if is linked with another equilibrium using global matrix
            bool  linked_with(const equilibrium  &,
                              const matrix<bool> &) const throw();

            //! display
            friend std::ostream & operator<<( std::ostream & , const cluster & );

            //! compile once built
            void compile(const xmlog            &xml,
                         const matrix<int>      &Nu,
                         const readable<double> &K,
                         const library          &lib,
                         const equilibria       &eqs,
                         equilibria             &all,
                         const unsigned          igv);
            

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
            const alist::ptr      alive;  //!< alive.size, fixed from the begining
            const auto_ptr<glist> group;  //!< group.size, initial+manifold
            const sp_tier::ptr    breed;  //!< category for species
            const eq_tier::ptr    genus;  //!< category for equilibria
            const udict::ptr      sDict;  //!< LUTs for species
            const udict::ptr      eDict;  //!< LUTs for equilibria
            const claws::ptr      canon;  //!< conservation laws to follow
            const cl_groups::ptr  clamp;  //!< groups of independent laws, for custodian
            const gvector::ptr    cross;  //!< number of cross/mixed equilibria per degree from manifold
            const unsigned        gvidx;  //!< graphivz index

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
            std::ostream &      display(std::ostream &) const;
            void                collect_alive();
            void                create_system();
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
            
        };


        class clusters : public object, public cxx_list_of<cluster>
        {
        public:
            explicit clusters() throw();
            virtual ~clusters() throw();
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(clusters);
        };
        
    }
    
}

#endif

