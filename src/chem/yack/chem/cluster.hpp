//! \file

#ifndef YACK_CHEM_CLUSTER_INCLUDED
#define YACK_CHEM_CLUSTER_INCLUDED 1

#include "yack/data/list/meta.hpp"
#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // definitions for cluster
        //
        //______________________________________________________________________
        typedef meta_node<const equilibrium> mnode; //!< alias
        typedef meta_list<const equilibrium> mlist; //!< alias

        //______________________________________________________________________
        //
        //
        //! cluster of linked equilibria
        //
        //______________________________________________________________________
        class cluster : public object, public mlist
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup with first equilibrium
            explicit cluster(const size_t       i,
                             const equilibrium &first,
                             const size_t       w);

            //! cleanup
            virtual ~cluster() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! check if connected to current cluster
            bool connected_to(const equilibrium &eq) const throw();

            //! set sub index for equilibria
            void finalize() throw();

            //! display
            friend std::ostream & operator<<(std::ostream &, const cluster &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t indx;  //!< index of cluster [1..]
            cluster     *next;  //!< for list
            cluster     *prev;  //!< for list
            const size_t width; //!< from equilibria

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
        };

        //______________________________________________________________________
        //
        //
        //! definitions of clusters
        //
        //______________________________________________________________________
        typedef cxx_list_of<cluster> clusters;


    }
}

#endif

