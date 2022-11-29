//! \file

#ifndef YACK_NEXUS_CLUSTER_INCLUDED
#define YACK_NEXUS_CLUSTER_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        class cluster : public object, public eq_team
        {
        public:
            explicit cluster() throw();
            virtual ~cluster() throw();

            //! true if eq is attached to one of the team
            bool attached_to(const equilibrium &eq) const throw();

            //! true if one of the equilibrium is attached
            bool attached_to(const cluster &rhs) const throw();


            cluster *next;
            cluster *prev;

            friend std::ostream & operator<<(std::ostream &os, const cluster &self)
            {
                const eq_node *node = self.head;
                if(node)
                {
                    os << "{ " << (**node).name;
                    for(node=node->next;node;node=node->next)
                    {
                        os << ", " << (**node).name;
                    }
                    os << " }";
                    return os;
                }
                else
                {
                    return os << "{}";
                }
            }

            void sort();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
        };

        typedef cxx_list_of<cluster> clusters_;

        class clusters : public clusters_
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


