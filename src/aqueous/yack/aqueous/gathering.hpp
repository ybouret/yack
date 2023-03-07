//! \file

#ifndef YACK_AQUEOUS_GATHERING_INCLUDED
#define YACK_AQUEOUS_GATHERING_INCLUDED 1

#include "yack/aqueous/entity.hpp"

namespace yack
{
    namespace aqueous
    {
        class gathering
        {
        public:
            virtual ~gathering() noexcept;
            explicit gathering() noexcept;

            void     update(const entity &) noexcept;

            std::ostream & pad(std::ostream &, const entity &) const;

            const size_t length; //!< max name length

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(gathering);
        };
    }

}

#endif

