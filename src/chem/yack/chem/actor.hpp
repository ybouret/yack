
//! \file

#ifndef YACK_CHEM_ACTOR_INCLUDED
#define YACK_CHEM_ACTOR_INCLUDED 1

#include "yack/chem/component.hpp"
#include "yack/type/authority.hpp"

namespace yack
{

    namespace chemical
    {

        class actor : public object, public authority<const species>
        {
        public:
            virtual ~actor() throw();
            explicit actor(const component &c) throw();

            const size_t nu;   //!< |c.nu| > 0
            const size_t nu1;  //!< nu-1
            actor       *next; //!< for list
            actor       *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actor);
        };

        typedef cxx_list_of<actor> actors;


    }

}

#endif

