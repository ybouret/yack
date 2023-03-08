//! \file

#ifndef YACK_AQUEOUS_COMPONENT_INCLUDED
#define YACK_AQUEOUS_COMPONENT_INCLUDED 1

#include "yack/aqueous/actors.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! a component is a species with a SIGNED coefficient
        //
        //______________________________________________________________________
        class component : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef ark_ptr<string,component> ptr; //!< shared pointer

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit component(const species &, const int) noexcept; //!< setup
            virtual ~component()                           noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string  & key()       const noexcept; //!< sp.key()
            const species & operator*() const noexcept; //!< access species

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const int      nu; //!< nu!=0 coefficient

        private:
            const species &sp;
            YACK_DISABLE_COPY_AND_ASSIGN(component);
        };
        
    }

}


#endif

