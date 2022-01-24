
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
                typedef jive::lexeme lexeme;
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
                virtual void on_terminal(const lexeme &);               //!< call with name/data
                virtual void on_internal(const string &, const size_t); //!< call with name/size


                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                void         walk(const xnode &tree); //!< init/inspect/quit


            protected:
                unsigned depth;                              //!< helper for nesting level
                std::ostream & indent(std::ostream &) const; //!< helper for tracing

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(translator);
                void         inspect(const xnode *node);    //!< recursive call
                virtual void on_init();                     //!< prepare resources
                virtual void on_quit() throw();             //!< free resources
            };
        }

    }

}

#endif
