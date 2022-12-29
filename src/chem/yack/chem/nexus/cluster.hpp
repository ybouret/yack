//! \file

#ifndef YACK_NEXUS_CLUSTER_INCLUDED
#define YACK_NEXUS_CLUSTER_INCLUDED 1

#include "yack/chem/eqs/tier.hpp"
#include "yack/chem/eqs/squad.hpp"
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

            bool attached_to(const equilibrium &eq) const throw(); //!< true if eq is attached to one of the team
            bool attached_to(const cluster &rhs)    const throw(); //!< true if one of the equilibrium is attached
            void sort();                                           //!< according to equilibrium index, collect working

            void build_army_with(const matrix<bool> &detached, const xmlog &xml); //!< detached equilibria for steady-state
            void build_wing_with(const matrix<bool> &detached, const xmlog &xml); //!< detached equilibria for balancing

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            cluster                   *next;    //!< for list/pool
            cluster                   *prev;    //!< for list
            const canon_type           canon;   //!< conservation among species
            const cells_type           cells;   //!< teams of linked laws
            const eq_tier::pointer     genus;   //!< roaming/bounded
            const eq_squads::pointer   army;    //!< full combination for steady state
            const eq_squads::pointer   wing;    //!< bounded combination for balance


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


