//! \file
#ifndef YACK_CHEM_SPECIES_INCLUDED
#define YACK_CHEM_SPECIES_INCLUDED 1


#include "yack/string.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/associative/suffix/set.hpp"

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
            typedef ark_ptr<string,const species> pointer; //!< alias
            typedef suffix_set<string,pointer>    set;     //!< alias for database
            typedef set::knot_type                knot;    //!< alias for fast access

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~species() throw(); //!< cleanup

            //! setup with name+algebraic charge
            template <typename NAME> inline
            explicit species(const NAME &user_n, const unit_t user_z) :
            name(user_n), z(user_z), indx(0), rank(0)
            {
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string &key() const throw(); //!< for pointer

            //! display
            friend std::ostream & operator<<(std::ostream &, const species &);



            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;   //!< unique name
            const unit_t z;      //!< algebraic charge
            const size_t indx;   //!< unique index in library
            const size_t rank;   //!< 0: spectator 1:primary >1:secondary
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(species);
        };

        //! alias
        typedef species::knot snode;

    }

}

#endif

