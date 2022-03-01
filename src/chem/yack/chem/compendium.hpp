//! \file


#ifndef YACK_CHEM_COMPENDIUM_INCLUDED
#define YACK_CHEM_COMPENDIUM_INCLUDED 1

#include "yack/large-object.hpp"
#include "yack/container/readable.hpp"
#include <iostream>

namespace yack
{
    namespace chemical
    {

        class entity;

        class compendium : public large_object
        {
        public:
            virtual ~compendium() throw();

            void           update_with(const entity &) throw();
            std::ostream & pad(std::ostream &,const entity &) const;




        protected:
            explicit compendium() throw();

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(compendium);

        public:
            const size_t width;

        };
    }
}

#endif

