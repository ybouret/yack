
//! \file

#ifndef YACK_CHEM_ACTORS_INCLUDED
#define YACK_CHEM_ACTORS_INCLUDED 1

#include "yack/chem/eqs/actor.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! base class for Actors
        //
        //______________________________________________________________________
        typedef cxx_list_of<Actor> Actors_;

        //______________________________________________________________________
        //
        //
        //! List of Actors with a naming system
        //
        //______________________________________________________________________
        class Actors :  public Actors_, public Entity
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Actors();
            Actors(const Actors &);
            virtual ~Actors() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const Actors &); //!< display
            void   makeName();                                                //!< create name from list

            
        private:
            YACK_DISABLE_ASSIGN(Actors);
        };

    }

}

#endif

