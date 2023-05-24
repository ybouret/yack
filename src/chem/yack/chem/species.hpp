//! \file

#ifndef YACK_CHEM_SPECIES_INCLUDED
#define YACK_CHEM_SPECIES_INCLUDED 1

#include "yack/chem/com/entity.hpp"
#include "yack/object.hpp"
#include "yack/spot-object.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{
    namespace Chemical
    {


        class Species : public spot_object, public  Entity, public counted
        {
        public:
            typedef ark_ptr<string,Species> Pointer;

            template <typename NAME> inline
            explicit Species(const NAME &id,
                             const int   zz) :
            Entity(id),
            z(zz) {}

            virtual ~Species() noexcept;

            friend std::ostream & operator<<(std::ostream &, const Species &);

            
            const int z;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Species);

        };
    }

}

#endif

