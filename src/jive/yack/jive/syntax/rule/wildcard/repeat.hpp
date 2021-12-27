


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
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const uint32_t mark = YACK_FOURCC('R','E','P',':'); //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! cleanup
                virtual ~repeat() throw();
                
                //! setup
                explicit repeat(const tag      &name_,
                                const rule     &host_,
                                const size_t    nmin) throw();

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________
                //! accept if host accepted >= count times
                virtual bool accept(YACK_JIVE_RULE_ARGS) const;

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t count; //!< minimal count
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(repeat);
            };
            
        }
        
    }
    
}

#endif
