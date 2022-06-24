//! \file

#ifndef YACK_CHEM_CLUSTER_INCLUDED
#define YACK_CHEM_CLUSTER_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // typedef for cluster
        //
        //______________________________________________________________________
        typedef meta_list<const equilibrium> vlist; //!< virtual list
        typedef vlist::node_type             vnode; //!< virtual node

        //______________________________________________________________________
        //
        //
        //! cluster of equilibria
        //
        //______________________________________________________________________
        class cluster : public object, public vlist
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef cxx_list_of<cluster> list; //!< alias


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cluster() throw(); //!< setup
            virtual ~cluster() throw(); //!< cleanup
            cluster(const cluster &);   //!< copy
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            void update()                           throw(); //!< sort by increasing equilibrium index
            bool matches(const cluster &)     const throw(); //!< both size and indices
            bool carries(const equilibrium &) const throw(); //!< check ownership
            bool includes(const cluster &)    const throw(); //!< carries all other's equilibria
            bool accepts(const equilibrium &) const throw(); //!< if attached to one of ours

            //! output as a list
            friend std::ostream & operator<<(std::ostream &, const cluster &);

            //! comparison for merge sort
            static int compare(const cluster *lhs, const cluster *rhs) throw();

            //__________________________________________________________________
            //
            // members
            //_________________________________________________________________
            cluster *next; //!< for list
            cluster *prev; //!< for list

        private:
            YACK_DISABLE_ASSIGN(cluster);
        };

    }

}

#endif
