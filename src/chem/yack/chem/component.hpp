//! \file

#ifndef YACK_CHEMICAL_COMPONENT_INCLUDED
#define YACK_CHEMICAL_COMPONENT_INCLUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! component=species+stoichio
        //
        //______________________________________________________________________
        class component : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and defintions
            //__________________________________________________________________
            typedef ark_ptr<string,const component> pointer; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            virtual ~component() noexcept;
            
            //! setup with |cf|>0 and a persistent species
            explicit component(const species &sr,
                               const int      cf) noexcept;
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string  & key()       const noexcept; //!< species key()
            const species & operator*() const noexcept; //!< access species

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
        private:
            const species &sp; //!< persistent species
            
        public:
            const int      nu; //!< |nu|>0
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(component);
        };
    }
    
}

#endif

