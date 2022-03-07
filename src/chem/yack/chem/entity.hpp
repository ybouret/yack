//! \file

#ifndef YACK_CHEM_ENTITY_INCLUDED
#define YACK_CHEM_ENTITY_INCLUDED 1

#include "yack/string.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! indexed and name entity
        //
        //______________________________________________________________________
        class entity : public large_object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            template <typename ID> inline
            entity(const ID &uuid, const size_t ipos) :
            name(uuid),
            indx(ipos)
            {
                assert(indx>0);
            }

            //! cleanup
            virtual ~entity() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t        operator*() const throw(); //!< access index
            const string &key()       const throw(); //!< access name


            //! access array
            template <typename T> inline
            const T & operator[](const readable<T> &arr) const throw()
            {
                assert(indx>=1);
                assert(indx<=arr.size());
                return arr[indx];
            }

            //! access array
            template <typename T> inline
            T & operator[](writable<T> &arr) const throw()
            {
                assert(indx>=1);
                assert(indx<=arr.size());
                return arr[indx];
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< uuid

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(entity);
            const size_t indx; //!< uuid
        };
    }
}

#endif

