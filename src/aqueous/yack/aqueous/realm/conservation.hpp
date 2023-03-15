//! \file

#ifndef YACK_AQUEOUS_CONSERVATION_INCLUDED
#define YACK_AQUEOUS_CONSERVATION_INCLUDED 1

#include "yack/aqueous/eqs/actors.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! conservation law
        //
        //______________________________________________________________________
        class conservation : public object, public actors_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit conservation() noexcept; //!< setup
            virtual ~conservation() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            YACK_PROTO_OSTREAM(conservation); //!< display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            conservation *next; //!< for list
            conservation *prev; //!< for list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conservation);
        };

        //______________________________________________________________________
        //
        //
        //! conservation law
        //
        //______________________________________________________________________
        typedef cxx_list_of<conservation> conservations;

    }
}

#endif

