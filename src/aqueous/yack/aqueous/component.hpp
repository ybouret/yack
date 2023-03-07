//! \file

#ifndef YACK_AQUEOUS_COMPONENT_INCLUDED
#define YACK_AQUEOUS_COMPONENT_INCLUDED 1

#include "yack/aqueous/actors.hpp"

namespace yack
{
    namespace aqueous
    {

        class component : public object, public counted
        {
        public:
            typedef ark_ptr<string,component> ptr;
            explicit component(const species &, const int) noexcept;
            virtual ~component()                           noexcept;

            const string  & key()       const noexcept;
            const species & operator*() const noexcept;
            
            const int      nu;
        private:
            const species &sp;
            YACK_DISABLE_COPY_AND_ASSIGN(component);
        };
        
    }

}


#endif

