//! \file

#ifndef YACK_CHEM_EQUILIBRIUM_INCLUDED
#define YACK_CHEM_EQUILIBRIUM_INCLUDED 1

#include "yack/chem/eqs/components.hpp"

namespace yack
{
    namespace Chemical
    {

        class Equilibrium : public spot_object, public counted, public Entity, public Components
        {
        public:
            typedef ark_ptr<string,Equilibrium> Pointer;

            virtual ~Equilibrium() noexcept;

        protected:
            template <typename NAME> inline
            Equilibrium(const NAME &id) : Entity(id)
            {
            }
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equilibrium);
        };

    }

}

#endif

