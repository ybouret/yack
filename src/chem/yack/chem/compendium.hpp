//! \file


#ifndef YACK_CHEM_COMPENDIUM_INCLUDED
#define YACK_CHEM_COMPENDIUM_INCLUDED 1

#include "yack/large-object.hpp"
#include "yack/container/readable.hpp"
#include "yack/latch.hpp"
#include <iostream>

namespace yack
{
    namespace chemical
    {

        class entity;

        //______________________________________________________________________
        //
        //
        //! base class for set of species/equilibria
        //
        //______________________________________________________________________
        class compendium : public large_object, public collection, public latch
        {
        public:
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void           update_with(const entity &)      throw(); //!< update width
            std::ostream & pad(std::ostream &,const entity &) const; //!< pad width-entity.name.size()


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~compendium() throw(); //!< cleanup

        protected:
            explicit compendium() throw(); //!< setup

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(compendium);

        public:
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t width; //!< max name width

        };
    }
}

#endif

