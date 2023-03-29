//! \file

#ifndef YACK_AQUEOUS_CONSERVED_INCLUDED
#define YACK_AQUEOUS_CONSERVED_INCLUDED 1

#include "yack/aqueous/realm/conservation.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! base class for conserved
        //
        //______________________________________________________________________
        typedef core_repo<const conservation> conserved_;

        //______________________________________________________________________
        //
        //
        //! group of dependent conservation laws
        //
        //______________________________________________________________________
        class conserved : public object, public conserved_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit conserved(const conservation &first); //!< setup with first
            virtual ~conserved() noexcept;                 //!< cleanup

            bool accepts(const conservation &) const noexcept; //!< with common species
            bool accepts(const conserved    &) const noexcept; //!< with common species
            
            //__________________________________________________________________
            //
            //  members
            //__________________________________________________________________
            conserved *next; //!< for list
            conserved *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conserved);
        };

      
    }

}

#endif

