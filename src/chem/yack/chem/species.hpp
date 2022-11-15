//! \file

#ifndef YACK_CHEMICAL_SPECIES_INCLUDED
#define YACK_CHEMICAL_SPECIES_INCLUDED 1

#include "yack/chem/entity.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! species=name+algebraic charge
        //
        //______________________________________________________________________
        class species : public entity
        {
        public:
            //__________________________________________________________________
            //
            // types and defintions
            //__________________________________________________________________
            typedef ark_ptr<string,const species> pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~species() throw(); //!< cleanup
            species(const species &);   //!< copy

            //! initialize with valid index
            template <typename NAME> inline
            explicit species(const NAME   &uid,
                             const int     zzz,
                             const size_t  idx) :
            entity(uid,idx), z(zzz), rank(0)
            {
            }

            //! write formated concentration
            std::ostream &  frame(std::ostream &os, const readable<double> &C) const;

            //! write formated concentration
            ios::ostream &  frame(ios::ostream &os, const readable<double> &C) const;

            //__________________________________________________________________
            //
            // memners
            //__________________________________________________________________
            const int    z;    //!< algebraic charge
            const size_t rank; //!< number of use
            
        private:
            YACK_DISABLE_ASSIGN(species);
        };

    }
}

#endif

