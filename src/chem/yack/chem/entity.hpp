//! \file

#ifndef YACK_CHEMICAL_ENTITY_INCLUDED
#define YACK_CHEMICAL_ENTITY_INCLUDED 1

#include "yack/string.hpp"
#include "yack/counted.hpp"

namespace yack
{
    namespace chemical
    {
        class entity : public object, public counted
        {
        public:
            virtual ~entity() throw();
            template <typename NAME> inline
            explicit entity(const NAME  &uid,
                            const size_t idx) :
            object(),
            counted(),
            name(uid),
            indx(idx)
            {
                assert(indx>0);
            }
            
            entity(const entity &);

            size_t operator*()   const throw();
            const string & key() const throw();

            const string name;

        private:
            const size_t indx;
            YACK_DISABLE_ASSIGN(entity);
        };
    }

}

#endif
