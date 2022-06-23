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
            explicit clusters() throw() : cluster::list() {}
            virtual ~clusters() throw() {}
            void     create_from(const equilibrium &eq)
            {
                cluster *cc = push_back( new cluster() );
                (*cc) << &eq;
            }


            friend std::ostream & operator<<(std::ostream &os, const clusters &CC)
            {
                os << "<clusters count=\"" << CC.size << "\">" << std::endl;
                for(const cluster *cls=CC.head;cls;cls=cls->next)
                {
                    os << "  " << *cls << std::endl;
                }
                os << "<clusters>";
                return os;
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(clusters);
        };
    }

}

#endif
