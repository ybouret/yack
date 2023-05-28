//! \file

#ifndef YACK_WEASEL_PARSER_INCLUDED
#define YACK_WEASEL_PARSER_INCLUDED 1


#include "yack/jive/parser.hpp"
#include "yack/chem/weasel/types.hpp"

namespace yack
{
    namespace Chemical
    {

        namespace Weasel
        {
            //__________________________________________________________________
            //
            //
            //! Parser for parsing species and equilibria
            //
            //__________________________________________________________________
            class Parser : public jive::parser
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Parser();           //!< setup
                virtual ~Parser() noexcept;  //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! parse and post-process input
                XNode * operator()(jive::module *);



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(Parser);

                //! remove all the optional '+'
                void cleanAST(XNode *);
            };
        }

    }
}

#endif
