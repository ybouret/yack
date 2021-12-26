
//! \file

#ifndef YACK_JIVE_SYNTAX_WILDCARD_INCLUDED
#define YACK_JIVE_SYNTAX_WILDCARD_INCLUDED 1

#include "yack/jive/syntax/rule/internal.hpp"
#include "yack/type/authority.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            
            //__________________________________________________________________
            //
            //
            //! wildcard base class
            //
            //__________________________________________________________________
            class wildcard : public internal, public authority<const rule>
            {
            public:
                virtual ~wildcard() throw(); //!< cleanup
                
            protected:
                //! setup with name, uuid and host
                explicit wildcard(const tag      &name_,
                                  const uint32_t  uuid_,
                                  const rule     &host_);
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(wildcard);
            };
            
        }
        
    }
    
}

#endif
