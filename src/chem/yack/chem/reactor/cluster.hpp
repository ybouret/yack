//! \file

#ifndef YACK_CHEMICAL_CLUSTER_INCLUDED
#define YACK_CHEMICAL_CLUSTER_INCLUDED 1

#include "yack/chem/reactor/eq-tier.hpp"
#include "yack/chem/active.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"

namespace yack {
    
    namespace chemical {


        typedef core_repo<const anode> sp_group;
        typedef sp_group::node_type    sp_gnode;

        class sp_tier : public object
        {
        public:
            typedef auto_ptr<const sp_tier> ptr;

            explicit sp_tier() : conserved( new sp_group() ), unbounded( new sp_group() ) {}
            virtual ~sp_tier() throw() {}

            const auto_ptr<sp_group> conserved;
            const auto_ptr<sp_group> unbounded;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(sp_tier);
        };

        

        //______________________________________________________________________
        //
        //
        //! cluster of linked species and equilibria
        //
        //______________________________________________________________________
        class cluster : public object, public latch
        {
        public:
            static const char * const clid; //!< "chemical::cluster"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cluster(const equilibrium &first);
            virtual ~cluster() throw();

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

            void compile(const xmlog &, const unsigned);
            

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void viz(ios::ostream &fp) const;

            //__________________________________________________________________
            //
            // member
            //__________________________________________________________________
            cluster              *next;   //!< for clusters
            cluster              *prev;   //!< for list
            const alist::ptr      alive;  //!< alive.size = M
            const auto_ptr<glist> group;  //!< group.size = N
            const eq_tier::ptr    genus;  //!< category for equilibria
            const sp_tier::ptr    breed;  //!< category for species
            const unsigned        gvidx;  //!< graphivz index

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
            std::ostream & display(std::ostream &) const;
            void collect_alive();
            void create_system();
            void conservations(const xmlog &xml);
            
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

