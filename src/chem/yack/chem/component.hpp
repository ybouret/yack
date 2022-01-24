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

            virtual ~component() throw();
            explicit component(const species::pointer &sp_,
                               const unit_t            nu_);

            const string &key() const throw();

            const species::pointer sp;
            const unit_t           nu;

        private:
            YACK_DISABLE_ASSIGN(component);
        };

    }

}

#endif
