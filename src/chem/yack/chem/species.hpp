//! \file

#ifndef YACK_CHEM_SPECIES_INCLUDED
#define YACK_CHEM_SPECIES_INCLUDED 1

#include "yack/chem/entity.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! species
        //
        //______________________________________________________________________
        class species : public entity
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef ark_ptr<string,const species> pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            template <typename ID>
            explicit species(const ID    &uuid,
                             const int    zz,
                             const size_t ipos) :
            entity(uuid,ipos),
            z(zz),
            rank(0)
            {
            }

            //! cleanup
            virtual ~species() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const int    z;    //!< algebraic charge
            const size_t rank; //!< referred in components

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(species);
        };

    }

}

#endif
