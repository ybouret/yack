
//! \file

#ifndef YACK_CHEMICAL_DESIGNER_SP_INFO_INCLUDED
#define YACK_CHEMICAL_DESIGNER_SP_INFO_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{
    namespace chemical
    {

        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! info for species identification
            //
            //__________________________________________________________________
            class sp_info
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                sp_info();                //!< setup empty/0
                sp_info(const sp_info &); //!< copy
                ~sp_info() throw();      //!< cleanup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                string name; //!< species name
                int    z;    //!< species charge

            private:
                YACK_DISABLE_ASSIGN(sp_info);
            };
        }

    }

}

#endif