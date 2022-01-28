
//! \file

#ifndef YACK_CHEM_ACTORS_INCLUDED
#define YACK_CHEM_ACTORS_INCLUDED 1

#include "yack/chem/actor.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! base class for actors
        //
        //______________________________________________________________________
        typedef cxx_list_of<actor> actors_;


        //______________________________________________________________________
        //
        //
        //! list of actors
        //
        //______________________________________________________________________
        class actors : public actors_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit actors() throw(); //!< setup
            virtual ~actors() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! K * law of mass action
            double   mass_action(double                  K,
                                 const readable<double> &C) const throw();

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
        };

        
    }

}

#endif

