
//! \file

#ifndef YACK_CHEM_LIBRARY_INCLUDED
#define YACK_CHEM_LIBRARY_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef suffix_set<string,Species::Pointer> SpecDB; //!< alias
        typedef SpecDB::knot_type                   sNode;     //!< alias
        typedef list_of<sNode>                      sList;     //!< alias

        //______________________________________________________________________
        //
        //
        //! constant equilibrium
        //
        //______________________________________________________________________
        class Library : public Gathering
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char CLID[]; //!< "Chemical::Libary"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Library() noexcept; //!< setup
            virtual ~Library() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const sList * operator->() const throw(); //!< access

            //! insert a new/multiple species
            template <typename NAME>
            const Species & operator()(const NAME &id,
                                       const int   zz)
            {
                return inserted( new Species(id,zz) );
            }

            const Species * query(const string &) noexcept; //!< query by name
            const Species * query(const char   *);          //!< query by name
            

        private:
            YACK_DISABLE_ASSIGN(Library);
            SpecDB sdb;

            const Species & inserted(Species *sp);
        };
    }

}

#endif

