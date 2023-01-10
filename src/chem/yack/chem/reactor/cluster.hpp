
#ifndef YACK_CHEMICAL_CLUSTER_INCLUDED
#define YACK_CHEMICAL_CLUSTER_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/chem/sub-list.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"

namespace yack {
    
    namespace chemical {


        typedef sub_node<equilibrium> gnode;
        typedef sub_list<equilibrium> glist;

        class cluster : public object
        {
        public:
            explicit cluster(const equilibrium &first);
            virtual ~cluster() throw();

            const list_of<gnode> * operator->() const throw();
            const list_of<gnode> & operator*()  const throw();

            bool  owns(const equilibrium &) const throw(); //!< mostly to debug
            void  grow(const equilibrium &);               //!< append to group

            //! check if is linked with another equilibrium using global matrix
            bool  linked_with(const equilibrium  &eq,
                              const matrix<bool> &related) const throw();


            inline friend std::ostream & operator<<( std::ostream &os, const cluster &self)
            {
                return self.display(os);
            }

            cluster              *next;
            cluster              *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
            const auto_ptr<glist> group;
            std::ostream & display(std::ostream &) const;
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

