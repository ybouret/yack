
//! \file

#ifndef YACK_JIVE_SYNTAX_TRANSLATOR_INCLUDED
#define YACK_JIVE_SYNTAX_TRANSLATOR_INCLUDED 1

#include "yack/jive/syntax/xnode.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            //__________________________________________________________________
            //
            //
            //! base class for translator
            //
            //__________________________________________________________________
            class translator
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit translator() throw(); //!< setup
                virtual ~translator() throw(); //!< cleanup

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual void on_terminal(const string &, const token&); //!< call with name/data
                virtual void on_internal(const string &, const size_t); //!< call with name/size

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                void         walk(const xnode &tree); //!< recursive travelling


            protected:
                unsigned depth;                              //!< helper for nesting level
                std::ostream & indent(std::ostream &) const; //!< helper for tracing

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(translator);
                void inspect(const xnode *node);
                
            };
        }

    }

}

#endif
