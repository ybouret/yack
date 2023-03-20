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

        //! index level
        enum index_level
        {
            top_level = 0,  //!< global/realm
            sub_level = 1,  //!< local/domain
            cat_level = 2   //!< for category
        };

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
                coerce(indx[0]) = coerce(indx[1])  = coerce(indx[2]) = idx;
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
            const size_t indx[3];   //!< 0:global 1:local


        private:
            YACK_DISABLE_ASSIGN(entity);
        };

    }
}

#endif

