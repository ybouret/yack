//! \file

#ifndef YACK_CHEM_COMPONENT_INCLUDED
#define YACK_CHEM_COMPONENT_INCLUDED 1

#include "yack/chem/species.hpp"


namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! a species with an algebraic coefficient
        //
        //______________________________________________________________________
        class component : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef ark_ptr<string,const component> pointer; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit component(const species &sp, const int nu); //!< setup
            virtual ~component() throw();                        //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string  & key()       const throw(); //!< host.name
            const species & operator*() const throw(); //!< access species

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const int       coef; //!< |coef| > 0

        private:
            const species  &host;
            YACK_DISABLE_COPY_AND_ASSIGN(component);
        };
    }

}

#endif

