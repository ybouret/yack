
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
            // access
            //__________________________________________________________________
            const list_of<gnode> * operator->() const throw();
            const list_of<gnode> & operator*()  const throw();

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

            void compile(const xmlog &);
            

            //__________________________________________________________________
            //
            // member
            //__________________________________________________________________
            cluster              *next;
            cluster              *prev;
            const alist::ptr      act;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
            const auto_ptr<glist> grp;
            std::ostream & display(std::ostream &) const;
            void collect_active();
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

