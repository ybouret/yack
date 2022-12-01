
//! \file

#ifndef YACK_CHEMICAL_CLAW_INCLUDED
#define YACK_CHEMICAL_CLAW_INCLUDED 1

#include "yack/chem/actors.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! conservation law = actors
        //
        //______________________________________________________________________
        class conservation_law : public object, public actors
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit conservation_law() throw(); //!< setup
            virtual ~conservation_law() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! specific display
            friend std::ostream & operator<<(std::ostream &os, const conservation_law &self);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            conservation_law *next; //!< for list
            conservation_law *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conservation_law);
        };

        //______________________________________________________________________
        //
        //
        //! hold list of c-laws
        //
        //__________________________________________________________________
        typedef cxx_list_of<conservation_law> conservation_laws;

        typedef meta_list<const conservation_law> claw_team_;
        typedef claw_team_::node_type             claw_node;

        //! team of related conservation law(s)
        class claw_team : public object, public claw_team_
        {
        public:
            explicit claw_team() throw();
            virtual ~claw_team() throw();

            claw_team *next;
            claw_team *prev;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claw_team);
        };

        typedef cxx_list_of<claw_team> claw_teams;


    }

}

#endif

