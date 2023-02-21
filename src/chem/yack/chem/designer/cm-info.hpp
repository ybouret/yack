

//! \file

#ifndef YACK_CHEMICAL_DESIGNER_CM_INFO_INCLUDED
#define YACK_CHEMICAL_DESIGNER_CM_INFO_INCLUDED 1

#include "yack/chem/designer/sp-info.hpp"
#include "yack/sequence/list.hpp"

namespace yack
{
    namespace chemical
    {

        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! info for components identification
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
                virtual ~cm_info() noexcept; //!< cleanup
                cm_info(const cm_info &);   //!< copy

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                int     nu;                 //!< parsed coefficient, nu!=0
                
            private:
                YACK_DISABLE_ASSIGN(cm_info);
            };


            typedef list<cm_info> cm_infos; //!< argument for cm_linker.walk
        }

    }

}

#endif
