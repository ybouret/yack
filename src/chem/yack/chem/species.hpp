//! \file
#ifndef YACK_CHEM_SPECIES_INCLUDED
#define YACK_CHEM_SPECIES_INCLUDED 1


#include "yack/string.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/large-object.hpp"
#include "yack/randomized/fwd.hpp"

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
        class species : public large_object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef ark_ptr<string,const species> pointer;            //!< alias
            typedef suffix_set<string,pointer>    set;                //!< alias for database
            typedef set::knot_type                knot;               //!< alias for fast access
            static const  int                     pmin = -12;         //!< min power of ten
            static const  int                     pmax =  1;          //!< max power of ten
            static double concentration( randomized::bits &) throw(); //!< in [10^pmin:10^pmax]


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

            //! access by (check) index
            template <typename T> inline
            const T & operator()(const readable<T> &arr) const throw()
            {
                assert( index_in(arr) );
                return arr[indx];
            }

            //! access by (check) index
            template <typename T> inline
            T & operator()(writable<T> &arr) const throw()
            {
                assert( index_in(arr) );
                return arr[indx];
            }


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
            bool index_in(const collection &) const throw();

        };

        //! alias
        typedef species::knot snode;

    }

}

#endif

