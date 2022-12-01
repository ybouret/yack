
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

        typedef meta_list<const conservation_law> claw_team_; //!< base class
        typedef claw_team_::node_type             claw_node;  //!< alias

        //______________________________________________________________________
        //
        //
        //! team of related conservation law(s)
        //
        //______________________________________________________________________
        class claw_team : public object, public claw_team_
        {
        public:
            explicit claw_team() throw();
            virtual ~claw_team() throw();

            bool attached_to(const conservation_law &lhs) const throw()
            {
                for(const claw_node *node=head;node;node=node->next)
                {
                    const conservation_law &rhs = **node;
                    if(rhs.attached_to(lhs)) return true;
                }
                return false;
            }

            bool attached_to(const claw_team &team) const throw()
            {
                for(const claw_node *node=head;node;node=node->next)
                {
                    if(team.attached_to(**node)) return true;
                }
                return false;
            }


            claw_team *next;
            claw_team *prev;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claw_team);
        };

        typedef cxx_list_of<claw_team> claw_teams_;

        class claw_teams : public claw_teams_
        {
        public:
            explicit claw_teams() throw();
            virtual ~claw_teams() throw();

            void recruit(const conservation_law &law);
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claw_teams);
            bool merged(const conservation_law &law);
            void reduce() throw();
        };

    }

}

#endif

