
//! \file

#ifndef YACK_LEXICAL_INSTRUCTIONS_INCLUDED
#define YACK_LEXICAL_INSTRUCTIONS_INCLUDED 1

#include "yack/jive/lexical/directive.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            
            typedef ark_ptr<string,directive>      instruction;   //!< alias
            typedef suffix_set<string,instruction> instructions_; //!< alias

            //__________________________________________________________________
            //
            //
            //! set of instructions, one per scanner
            //
            //__________________________________________________________________
            class instructions : public object, public instructions_
            {
            public:
                explicit instructions() noexcept; //!< setup
                virtual ~instructions() noexcept; //!< cleanup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(instructions);
            };



        }

    }
}

#endif

