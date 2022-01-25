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
        //! component: algebraic coefficient and a registered species
        //
        //______________________________________________________________________
        class component : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef ark_ptr<string,const component> pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~component() throw();

            //! setup with a register species and a not null coeff
            explicit component(const species  &sp_,
                               const unit_t    nu_);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string &key() const throw();                                   //!< for components
            void          display(std::ostream &, const bool first)       const; //!< display
            friend std::ostream & operator<<(std::ostream &, const component &); //!< display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const species &sp; //!< species
            const unit_t   nu; //!< stoichiometry

        private:
            YACK_DISABLE_ASSIGN(component);
        };

    }

}

#endif
