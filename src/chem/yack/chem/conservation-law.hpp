
//! \file

#ifndef YACK_CHEMICAL_CLAW_INCLUDED
#define YACK_CHEMICAL_CLAW_INCLUDED 1

#include "yack/chem/actors.hpp"

namespace yack
{
    namespace chemical
    {
        class conservation_law : public object, public actors
        {
        public:
            explicit conservation_law() throw();
            virtual ~conservation_law() throw();

            friend std::ostream & operator<<(std::ostream &os, const conservation_law &self);


            conservation_law *next;
            conservation_law *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conservation_law);
        };

        typedef cxx_list_of<conservation_law> conservation_laws;

    }

}

#endif

