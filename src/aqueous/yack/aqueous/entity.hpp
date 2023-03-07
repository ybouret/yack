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
            explicit entity(NAME         &uuid,
                            const size_t  indx) :
            name(uuid),
            primary(indx),
            replica(indx)
            {
                assert(primary>0);
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
            const size_t primary;   //!< global index
            const size_t replica;   //!< index in cluster


        private:
            YACK_DISABLE_ASSIGN(entity);
        };

    }
}

#endif

