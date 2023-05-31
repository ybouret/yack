//! \file

#ifndef YACK_CHEM_COMPONENT_INCLUDED
#define YACK_CHEM_COMPONENT_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/chem/com/xtypes.hpp"

namespace yack
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! a Component is a Species with a SIGNED coefficient != 0
        //
        //______________________________________________________________________
        class Component : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef ark_ptr<string,Component> Pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Component(const int, const Species &sp) noexcept; //!< setup>
            virtual ~Component() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const Species & operator*() const noexcept; //!< access
            const string  & key()       const noexcept; //!< species name

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const int            nu; //!< coefficient != 0
            const Extended::Real xn; //!< nu as extended real
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Component);
            const Species &sp;
        };
    }

}

#endif
