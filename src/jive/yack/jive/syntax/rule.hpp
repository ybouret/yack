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
            enum rule_type
            {
                terminal_type, //!< a  terminal rule
                internal_type  //!< an internal rule
            };

            class xnode; //!< forward

            //__________________________________________________________________
            //
            //
            //! arguments for rule
            //
            //__________________________________________________________________
#define YACK_JIVE_RULE_ARGS source &src, lexer &lxr, xnode * &tree

            //__________________________________________________________________
            //
            //
            //! a syntax rule
            //
            //__________________________________________________________________
            class rule : public object, public ios::vizible
            {
            public:
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag       name; //!< unique name
                const rule_type type; //!< internal/terminal
                const uint32_t  uuid; //!< unique user ID
                rule           *next; //!< for list
                rule           *prev; //!< for list

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! accept method
                /**
                 upon success, accept, grow tree with produced node
                 */
                virtual bool accept(YACK_JIVE_RULE_ARGS) const = 0;

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
               
                //! cast w.r.t uuid
                template <typename CLASS> inline
                CLASS *as() throw()
                {
                    assert(uuid==CLASS::mark);
                    assert(NULL!=self);
                    return static_cast<CLASS *>(self);
                }

                //! cast w.r.t uuid, const
                template <typename CLASS> inline
                const CLASS *as() const throw()
                {
                    assert(uuid==CLASS::mark);
                    assert(NULL!=self);
                    return static_cast<const CLASS *>(self);
                }
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~rule() throw(); //!< cleanup

            protected:
                //! setup
                explicit rule(const tag      &name_,
                              const rule_type type_,
                              const uint32_t  uuid_) throw();

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

