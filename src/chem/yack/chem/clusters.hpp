//! \file

#ifndef YACK_CHEM_CLUSTERS_INCLUDED
#define YACK_CHEM_CLUSTERS_INCLUDED 1

#include "yack/chem/cluster.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! list of detached clusters
        //
        //______________________________________________________________________
        class clusters : public cluster::list
        {
        public:
            explicit clusters() throw(); //!< setup empty
            virtual ~clusters() throw(); //!< cleanup

            //! start a new cluster from single equilibrium
            void     createFrom(const equilibrium &eq);

            void update();    //!< update all
            void sort();      //!< sort
            void normalize(); //!< update all clusters, sort, and set info to cluster index

            //! display
            friend std::ostream & operator<<(std::ostream &, const clusters &);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(clusters);
        };
    }

}

#endif
