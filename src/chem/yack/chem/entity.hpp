//! \file

#ifndef YACK_CHEM_ENTITY_INCLUDED
#define YACK_CHEM_ENTITY_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{
    namespace chemical
    {

        class entity : public object, public counted
        {
        public:
            template <typename ID> inline
            entity(const ID &uuid, const size_t ipos) :
            name(uuid),
            indx(ipos)
            {
                assert(indx>0);
            }

            virtual ~entity() throw();

            size_t operator*() const throw();

            const string &key() const throw();


            template <typename T> inline
            const T & operator[](const readable<T> &arr) const throw()
            {
                assert(indx>=1);
                assert(indx<=arr.size());
                return arr[indx];
            }

            template <typename T> inline
            T & operator[](writable<T> &arr) const throw()
            {
                assert(indx>=1);
                assert(indx<=arr.size());
                return arr[indx];
            }

            const string name;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(entity);
            const size_t indx;
        };
    }
}

#endif

