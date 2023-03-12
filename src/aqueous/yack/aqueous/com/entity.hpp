//! \file

#ifndef YACK_AQUEOUS_ENTITY_INCLUDED
#define YACK_AQUEOUS_ENTITY_INCLUDED 1

#include "yack/string.hpp"
#include "yack/large-object.hpp"
#include "yack/proto-ostream.hpp"

namespace yack
{
    namespace aqueous
    {

        //______________________________________________________________________
        //
        //
        //! base class for enties: a name and two indices
        //
        //______________________________________________________________________
        class entity 
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~entity() noexcept;  //!< cleanup
            entity(const entity &other); //!< copy

            //! setup with name and index
            template <typename NAME> inline
            explicit entity(NAME         &uid,
                            const size_t  idx) :
            name(uid),
            indx()
            {
                assert(idx>0);
                coerce(indx[0]) = coerce(indx[1])  = idx;
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string & key() const noexcept; //!< key for ark_ptr

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;      //!< unique name
            const size_t indx[2];   //!< 0:global 1:local


        private:
            YACK_DISABLE_ASSIGN(entity);
        };

    }
}

#endif

