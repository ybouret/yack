//! \file

#ifndef YACK_CHEM_CLUSTER_INCLUDED
#define YACK_CHEM_CLUSTER_INCLUDED 1

#include "yack/data/list/meta.hpp"
#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        class plexus;

        //______________________________________________________________________
        //
        //
        // definitions for cluster
        //
        //______________________________________________________________________
        typedef meta_node<const equilibrium> mnode; //!< alias
        typedef meta_list<const equilibrium> mlist; //!< alias

        typedef meta_node<const species>     anode; //!< alias
        typedef meta_list<const species>     alist; //!< alias

        //______________________________________________________________________
        //
        //
        //! cluster of linked equilibria
        //
        //______________________________________________________________________
        class cluster : public large_object, public mlist
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

            //! check if species is used within equilria
            bool uses(const species &sp) const throw();

            //! set sub index for equilibria
            void finalize(const snode *) throw();

            //! display
            friend std::ostream & operator<<(std::ostream &, const cluster &);

            double variance(const readable<double> &C, const plexus &) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t indx;  //!< index of cluster [1..]
            const alist  used;  //!< used active species
            const alist  excl;  //!< from another cluster
            cluster     *next;  //!< for list
            cluster     *prev;  //!< for list
            const size_t width; //!< from equilibria



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
        };



    }
}

#endif

