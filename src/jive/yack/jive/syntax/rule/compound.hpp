//! \file

#ifndef YACK_JIVE_SYNTAX_COMPOUND_INCLUDED
#define YACK_JIVE_SYNTAX_COMPOUND_INCLUDED 1

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
            //! node for a rule
            //
            //__________________________________________________________________
            class component : public object, public authority<const rule>
            {
            public:
                virtual ~component()             throw(); //!< cleanup
                explicit component(const rule &) throw(); //!< setup
                component *next;                          //!< for manifest
                component *prev;                          //!< for manifest
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(component);
            };
            
            //__________________________________________________________________
            //
            //
            //! manifest of rules
            //
            //__________________________________________________________________
            typedef cxx_list_of<component> manifest;
            
            //__________________________________________________________________
            //
            //
            //! base class for compounds
            //
            //__________________________________________________________________
            class compound : public internal, public manifest
            {
            public:
                compound & operator<<(const rule &); //!< helper method
                virtual ~compound() throw();         //!< cleanup
            protected:
                explicit compound(const tag     &name_,
                                  const uint32_t uuid_) throw(); //!< setup
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(compound);
            };
            
        }
    }
}

#endif

