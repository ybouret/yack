//! \file
#ifndef YACK_CHEM_SPECIES_INCLUDED
#define YACK_CHEM_SPECIES_INCLUDED 1


#include "yack/string.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! names species
        //
        //______________________________________________________________________
        class species : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef ark_ptr<string,const species> pointer;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~species() throw(); //!< cleanup

            //! setup with name+algebraic charge
            template <typename NAME> inline
            explicit species(const NAME &user_n, const unit_t user_z) :
            name(user_n), z(user_z)
            {
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string &key() const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;   //!< unique name
            const unit_t z;      //!< algebraic charge


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(species);
        };

    }

}

#endif

