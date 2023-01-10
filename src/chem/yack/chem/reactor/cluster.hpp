
#ifndef YACK_CHEMICAL_CLUSTER_INCLUDED
#define YACK_CHEMICAL_CLUSTER_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/chem/active.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"

namespace yack {
    
    namespace chemical {


        typedef sub_node<equilibrium> gnode; //!< alias
        typedef sub_list<equilibrium> glist; //!< alias

        typedef core_repo<const gnode> eq_group_;
        typedef eq_group_::node_type    eq_gnode;

        class eq_group : public object, public eq_group_
        {
        public:
            explicit eq_group() throw() : object(), eq_group_() {}
            virtual ~eq_group() throw() {}

            friend std::ostream & operator<<(std::ostream &os, const eq_group &self)
            {
                os << "{ ";
                const eq_gnode *node=self.head;
                if(node)
                {
                    os << (***node).host.name;
                    for(node=node->next;node;node=node->next)
                    {
                        os << ", " << (***node).host.name;
                    }
                }
                return os << " }";
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_group) throw();
        };

        class eq_tier : public object
        {
        public:
            typedef auto_ptr<const eq_tier> ptr;

            explicit eq_tier() :
            bounded( new eq_group() ),
            roaming( new eq_group() ),
            special( new eq_group() )
            {}
            
            virtual ~eq_tier() throw() {}

            const auto_ptr<eq_group> bounded; //!< at least one conserved species in both sides
            const auto_ptr<eq_group> roaming; //!< prod only or reac only
            const auto_ptr<eq_group> special; //!< all reac or all prod, or both are unbounded

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_tier);
        };

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
            void collect_genus();
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

