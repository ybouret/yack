//! \file

#ifndef YACK_AQUEOUS_CONSERVATION_INCLUDED
#define YACK_AQUEOUS_CONSERVATION_INCLUDED 1

#include "yack/aqueous/eqs/actors.hpp"

namespace yack
{
    namespace aqueous
    {

        class conservation : public object, public actors_
        {
        public:
            explicit conservation() noexcept;
            virtual ~conservation() noexcept;

            YACK_PROTO_OSTREAM(conservation);

            conservation *next;
            conservation *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conservation);
        };

        typedef cxx_list_of<conservation> conservations;
    }
}

#endif

