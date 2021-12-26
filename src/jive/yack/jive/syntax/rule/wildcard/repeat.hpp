


//! \file

#ifndef YACK_JIVE_SYNTAX_REPEAT_INCLUDED
#define YACK_JIVE_SYNTAX_REPEAT_INCLUDED 1

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
            //! wildcard repeat
            //
            //__________________________________________________________________
            class repeat : public wildcard
            {
            public:
                static const uint32_t mark = YACK_FOURCC('R','E','P',':');
                
                //! cleanup
                virtual ~repeat() throw();
                
                //! setup
                explicit repeat(const tag      &name_,
                                const rule     &host_,
                                const size_t    nmin);
                
                //! minimal count
                const size_t count;
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(repeat);
            };
            
        }
        
    }
    
}

#endif
