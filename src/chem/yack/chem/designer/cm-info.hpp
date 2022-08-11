

//! \file

#ifndef YACK_CHEMICAL_DESIGNER_CM_INFO_INCLUDED
#define YACK_CHEMICAL_DESIGNER_CM_INFO_INCLUDED 1

#include "yack/chem/designer/sp-info.hpp"

namespace yack
{
    namespace chemical
    {

        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! info for componentidentification
            //
            //__________________________________________________________________
            class cm_info : public sp_info
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit cm_info();         //!< setup empty/1
                virtual ~cm_info() throw(); //!< cleanup
                cm_info(const cm_info &);   //!< copy

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                int     nu;
                
            private:
                YACK_DISABLE_ASSIGN(cm_info);
            };
        }

    }

}

#endif
