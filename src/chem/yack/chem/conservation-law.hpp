
//! \file

#ifndef YACK_CHEMICAL_CLAW_INCLUDED
#define YACK_CHEMICAL_CLAW_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/math/adder.hpp"

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
            // types and definitions
            //__________________________________________________________________
            typedef math::adder<double> adder_type;

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

            void   finalize(const size_t i) throw();                        //!< setup nrm2 and inx
            double evaluate(const readable<double> &C, adder_type &) const; //!< evaluate excess

            //! regulate
            /**
             - if evaluate() <=0, return false
             - if evaluate() > 0: copy source, correct target, load xadd, return treu
             */
            bool   regulate(writable<double>       &target,
                            const readable<double> &source,
                            adder_type             &xadd) const;

            size_t operator*() const throw() { return indx; }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            conservation_law *next; //!< for list
            conservation_law *prev; //!< for list


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conservation_law);
            const unsigned    nrm2; //!< sum of squared coefficients
            const size_t      indx; //!< index
        };

        //______________________________________________________________________
        //
        //
        //! hold list of c-laws
        //
        //______________________________________________________________________
        typedef cxx_list_of<conservation_law>     conservation_laws;
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
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit claw_team() throw(); //!< setup empty
            virtual ~claw_team() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool attached_to(const conservation_law &) const throw(); //!< check link to a law
            bool attached_to(const claw_team        &) const throw(); //!< check link to a team

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            claw_team *next; //!< for list
            claw_team *prev; //!< for list
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claw_team);
        };


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
        //! teams of interlinked conservation laws
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
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claw_teams);
            bool merged(const conservation_law &law);
            void reduce() throw();
        };

    }

}

#endif

