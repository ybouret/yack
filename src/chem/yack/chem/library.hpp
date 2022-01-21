//! \file

#ifndef YACK_CHEM_LIBRARY_INCLUDED
#define YACK_CHEM_LIBRARY_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! library of species
        //
        //______________________________________________________________________
        class library : public lockable
        {
        public:
            typedef suffix_set<string,species::pointer> db_type;
            typedef db_type::knot_type                  knot_type;

            explicit library();
            virtual ~library() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(library);

        };
    }

}

#endif

