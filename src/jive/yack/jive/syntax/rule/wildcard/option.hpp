

//! \file

#ifndef YACK_JIVE_SYNTAX_OPTION_INCLUDED
#define YACK_JIVE_SYNTAX_OPTION_INCLUDED 1

#include "yack/jive/syntax/rule/wildcard.hpp"
#include "yack/type/fourcc.h"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            //__________________________________________________________________
            //
            //
            //! wildcard option
            //
            //__________________________________________________________________
            class option : public wildcard
            {
            public:
                static const uint32_t mark = YACK_FOURCC('O','P','T',':');
                
                //! cleanup
                virtual ~option() throw();
                
                //! setup
                explicit option(const tag      &name_,
                                const rule     &host_);
                
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(option);
            };
            
        }
        
    }
    
}

#endif
