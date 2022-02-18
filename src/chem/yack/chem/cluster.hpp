//! \file

#ifndef YACK_CHEM_CLUSTER_INCLUDED
#define YACK_CHEM_CLUSTER_INCLUDED 1

#include "yack/data/list/meta.hpp"
#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {

        typedef meta_node<const equilibrium> mnode;
        typedef meta_list<const equilibrium> mlist;

        class cluster : public object, public mlist
        {
        public:
            explicit cluster() throw();
            virtual ~cluster() throw();

            bool connected_to(const equilibrium &eq) const throw();

            cluster *next;
            cluster *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
        };

        typedef cxx_list_of<cluster> clusters;


    }
}

#endif

