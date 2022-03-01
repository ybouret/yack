//! \file

#ifndef YACK_CHEM_COMPONENT_INCLUDED
#define YACK_CHEM_COMPONENT_INCLUDED 1

#include "yack/chem/species.hpp"


namespace yack
{
    namespace chemical
    {
        class component : public object, public counted
        {
        public:
            typedef ark_ptr<string,const component> pointer;

            explicit component(const species &sp, const int nu);
            virtual ~component() throw();

            const string  & key()       const throw();
            const species & operator*() const throw();
            const int       coef; //!< |coef| > 0

        private:
            const species  &host;
            YACK_DISABLE_COPY_AND_ASSIGN(component);
        };
    }

}

#endif

