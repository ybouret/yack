
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

        class compendium :
        public latch,
        public collection
        {
        public:
            virtual ~compendium() throw();

            std::ostream & pad(std::ostream &, const size_t n) const;
            std::ostream & pad(std::ostream &, const string &) const;
            
            const size_t width;

        protected:
            explicit compendium() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(compendium);

        };

    }

}

#endif
