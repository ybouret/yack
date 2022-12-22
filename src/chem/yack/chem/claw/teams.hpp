//! \file

#ifndef YACK_CHEMICAL_CLAW_TEAMS_INCLUDED
#define YACK_CHEMICAL_CLAW_TEAMS_INCLUDED 1

#include "yack/chem/claw/team.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! base class for teams of interlinked conservation laws
        //
        //______________________________________________________________________
        typedef cxx_list_of<claw_team> claw_teams_;


        //______________________________________________________________________
        //
        //
        //! independant teams for a cluster
        //
        //______________________________________________________________________
        class claw_teams : public claw_teams_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit claw_teams() throw(); //!< setup empty
            virtual ~claw_teams() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! recruit a law: create or add to a team, update structure
            void recruit(const conservation_law &law);


            //! update all
            void update(writable<double> &C0,
                        matrix<double>   &Qc,
                        writable<double> &Qs,
                        claw_repo        &cr,
                        raddops          &xadd) const
            {
                for(const claw_team *team=head;team;team=team->next)
                {
                    team->update(C0,Qc,Qs,cr,xadd);
                }
            }




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claw_teams);
            bool merged(const conservation_law &law);
            void reduce() throw();
        };
    }

}

#endif

