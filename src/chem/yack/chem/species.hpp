//! \file

#ifndef YACK_CHEM_SPECIES_INCLUDED
#define YACK_CHEM_SPECIES_INCLUDED 1

#include "yack/chem/entity.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{
    namespace chemical
    {

        class species : public entity
        {
        public:
            typedef ark_ptr<string,const species> pointer;

            template <typename ID>
            explicit species(const ID    &uuid,
                             const int    zz,
                             const size_t ipos) :
            entity(uuid,ipos),
            z(zz),
            rank(0)
            {
            }

            virtual ~species() throw();
            
            const int    z;
            const size_t rank;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(species);
        };

    }

}

#endif
