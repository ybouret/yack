//! \file

#ifndef YACK_CHEM_ENTITY_INCLUDED
#define YACK_CHEM_ENTITY_INCLUDED 1

#include "yack/ios/fmt/entity.hpp"

namespace yack
{
    namespace Chemical
    {

        enum IndexLevel
        {
            TopLevel = 0,
            SubLevel = 1
        };

        class Entity : public ios::entity
        {
        public:
            static const size_t Levels = SubLevel+1;

            template <typename NAME> inline
            explicit Entity(const NAME &id) : ios::entity(id), indx() { clr(); }
            virtual ~Entity() noexcept;
            
            const size_t indx[Levels];

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Entity);
            void clr() noexcept;
        };

        typedef Entity::group Gathering;

    }
}

#endif


