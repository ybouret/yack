//! \file

#ifndef YACK_CHEM_COMPONENT_INCLUDED
#define YACK_CHEM_COMPONENT_INCLUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace Chemical
    {
        class Component : public object, public counted
        {
        public:
            typedef ark_ptr<string,Component> Pointer;
            
            explicit Component(const int, const Species &sp) noexcept;
            virtual ~Component() noexcept;

            const Species & operator*() const noexcept;
            const string  & key()       const noexcept;

            const int      nu;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Component);
            const Species &sp;
        };
    }

}

#endif
