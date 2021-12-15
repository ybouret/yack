
//! \file

#ifndef YACK_LEXICAL_DIRECTIVE_INCLUDED
#define YACK_LEXICAL_DIRECTIVE_INCLUDED 1

#include "yack/jive/pattern.hpp"
#include "yack/functor.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            //__________________________________________________________________
            //
            //
            //! generic action alias
            //
            //__________________________________________________________________
            typedef functor<bool,TL1(const token &)> action;

            //__________________________________________________________________
            //
            //
            //! named directive for a given pattern
            //
            //__________________________________________________________________
            class directive : public object, public counted
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit directive(const tag &, const motif &, const action &); //!< setup
                virtual ~directive() throw();                                   //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const string & key() const throw(); //!< *uuid for instructions

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag    uuid; //!< unique user ID
                const motif  info; //!< pattern info
                const action duty; //!< task if chosen

                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(directive);
            };

            typedef ark_ptr<string,directive>      instruction;  //!< alias
            typedef suffix_set<string,instruction> instructions; //!< alias

        }

    }

}

#endif
