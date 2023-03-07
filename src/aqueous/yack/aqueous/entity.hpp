//! \file

#ifndef YACK_AQUEOUS_ENTITY_INCLUDED
#define YACK_AQUEOUS_ENTITY_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{
    namespace aqueous
    {

        class entity : public object
        {
        public:
            virtual ~entity() noexcept;

        private:
            YACK_DISABLE_ASSIGN(entity);
        };

    }
}

#endif

