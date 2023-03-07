//! \file

#ifndef YACK_AQUEOUS_ENTITY_INCLUDED
#define YACK_AQUEOUS_ENTITY_INCLUDED 1

#include "yack/string.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace aqueous
    {

        class entity : public object
        {
        public:
            virtual ~entity() noexcept;
            template <typename NAME> inline
            explicit entity(NAME         &uuid,
                            const size_t  gidx,
                            const size_t  cidx) :
            name(uuid),
            primary(gidx),
            replica(cidx)
            {
                assert(gidx>=1);
                assert(cidx>=1);
            }
            
            entity(const entity &other);

            const string name;      //!< unique name
            const size_t primary;   //!< global index
            const size_t replica;   //!< index in cluster


        private:
            YACK_DISABLE_ASSIGN(entity);
        };

    }
}

#endif

