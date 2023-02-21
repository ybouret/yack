//! \file

#ifndef YACK_CHEMICAL_CL_GROUPS_INCLUDED
#define YACK_CHEMICAL_CL_GROUPS_INCLUDED 1

#include "yack/chem/reactor/conservation/group.hpp"

namespace yack
{
    namespace chemical
    {
        

        //______________________________________________________________________
        //
        //
        //! groups of independent conservation lots
        //
        //______________________________________________________________________
        class clots : public object, public cxx_list_of<clot>
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const clots> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit clots() noexcept; //!< setup
            virtual ~clots() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! two-steps placement of a new c-law
            void recruit(const claw &cl);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(clots);
        };

    }

}

#endif
