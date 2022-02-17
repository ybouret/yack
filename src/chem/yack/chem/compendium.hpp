
//! \file

#ifndef YACK_CHEM_COMPENDIUM_INCLUDED
#define YACK_CHEM_COMPENDIUM_INCLUDED 1

#include "yack/latch.hpp"
#include "yack/container/collection.hpp"
#include "yack/string.hpp"

#include <iostream>

namespace yack
{

    namespace chemical
    {

        class entity;
        
        //______________________________________________________________________
        //
        //
        //! for library/equilibria base class
        //
        //______________________________________________________________________
        class compendium : public latch, public collection
        {
        public:
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            std::ostream & pad(std::ostream &, const size_t n) const; //!< n to width
            std::ostream & pad(std::ostream &, const string &) const; //!< n to width

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t width;  //!< max name length (species/equilibria)

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~compendium() throw(); //!< cleanup
        protected:
            explicit compendium() throw(); //!< setup

            //!update index of entity
            void     update(entity &, const size_t indx) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(compendium);

        };

    }

}

#endif
