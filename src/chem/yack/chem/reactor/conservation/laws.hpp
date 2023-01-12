//! \file

#ifndef YACK_CHEMICAL_CLAWS_INCLUDED
#define YACK_CHEMICAL_CLAWS_INCLUDED 1

#include "yack/chem/reactor/conservation/law.hpp"

namespace yack
{
    namespace chemical
    {
        
        //______________________________________________________________________
        //
        //
        //! base class for conservation laws
        //
        //______________________________________________________________________
        typedef cxx_list_of<claw> claws_;

        //______________________________________________________________________
        //
        //
        //! list of conservations laws
        //
        //______________________________________________________________________
        class claws : public object, public claws_
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const claws>  ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit claws() throw(); //!< setup
            virtual ~claws() throw(); //!< cleanup

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claws);
        };

     

    }
}

#endif

