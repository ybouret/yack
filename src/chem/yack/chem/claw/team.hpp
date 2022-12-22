//! \file

#ifndef YACK_CHEMICAL_CLAW_TEAM_INCLUDED
#define YACK_CHEMICAL_CLAW_TEAM_INCLUDED 1

#include "yack/chem/claw.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // aliases
        //
        //______________________________________________________________________
        typedef meta_list<const conservation_law> claw_team_; //!< base class
        typedef claw_team_::node_type             claw_node;  //!< alias
        typedef meta_repo<const conservation_law> claw_repo;  //!< alias

        //______________________________________________________________________
        //
        //
        //! team of related conservation law(s)
        //
        //______________________________________________________________________
        class claw_team : public object, public claw_team_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit claw_team() throw(); //!< setup empty
            virtual ~claw_team() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods to setup
            //__________________________________________________________________
            bool attached_to(const conservation_law &) const throw(); //!< check link to a law
            bool attached_to(const claw_team        &) const throw(); //!< check link to a team

            //__________________________________________________________________
            //
            // methods to solve
            //__________________________________________________________________
            void update(writable<double> &C0,
                        matrix<double>   &Qc,
                        writable<double> &Qs,
                        claw_repo        &cr,
                        raddops          &xadd) const;


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            claw_team *next; //!< for list
            claw_team *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claw_team);
        };

        
    }

}

#endif
