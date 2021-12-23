//! \file

#ifndef YACK_JIVE_SYNTAX_RULE_INCLUDED
#define YACK_JIVE_SYNTAX_RULE_INCLUDED 1

#include "yack/jive/lexical/analyzer.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            //__________________________________________________________________
            //
            //
            //! define the family of rules
            //
            //__________________________________________________________________
            enum family
            {
                is_terminal, //!< a  terminal rule
                is_internal  //!< an internal rule
            };


            //__________________________________________________________________
            //
            //
            //! a syntax rule
            //
            //__________________________________________________________________
            class rule : public object
            {
            public:
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag      name; //!< unique name
                const family   kind; //!< internal/terminal
                const uint32_t uuid; //!< unique user ID
                rule          *next; //!< for list
                rule          *prev; //!< for list

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~rule() throw(); //!< cleanup

            protected:
                //! setup
                explicit rule(const tag     &name_,
                              const family   kind_,
                              const uint32_t uuid_) throw();

                void *self; //!< most derived class

                //! helper to set address of most derived class
                template <typename CLASS> void I_am() throw()
                {
                    self = static_cast<CLASS *>(this);
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rule);
            };
        }
    }
}

#endif

