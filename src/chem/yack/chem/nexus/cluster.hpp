//! \file

#ifndef YACK_NEXUS_CLUSTER_INCLUDED
#define YACK_NEXUS_CLUSTER_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/chem/claw/teams.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! cluster of equilibria
        //
        //______________________________________________________________________
        class cluster :  public eq_team
        {
        public:
            typedef auto_ptr<const eq_team>           eq_team_ptr; //!< alias
            typedef auto_ptr<const claw_teams>        cells_type;  //!< alias
            typedef auto_ptr<const conservation_laws> canon_type;  //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cluster();         //!< setup emtpy
            virtual ~cluster() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! true if eq is attached to one of the team
            bool attached_to(const equilibrium &eq) const throw();

            //! true if one of the equilibrium is attached
            bool attached_to(const cluster &rhs) const throw();

            //! display as list
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

            void sort(); //!< according to equilibrium index

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            cluster                *next;    //!< for list/pool
            cluster                *prev;    //!< for list
            const canon_type        canon;   //!< conservation among species
            const cells_type        cells;   //!< teams of linked laws
            const eq_team_ptr       roaming; //!< roaming of this cluster
            const eq_team_ptr       bounded; //!< roaming of this cluster

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
        };

        //______________________________________________________________________
        //
        //
        //! base type for clusters
        //
        //______________________________________________________________________
        typedef cxx_list_of<cluster> clusters_;

        //______________________________________________________________________
        //
        //
        //! list of clusters
        //
        //______________________________________________________________________
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


