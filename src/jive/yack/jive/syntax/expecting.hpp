
//! \file

#ifndef YACK_JIVE_EXPECTING_INCLUDED
#define YACK_JIVE_EXPECTING_INCLUDED 1

#include "yack/jive/syntax/rule.hpp"
#include "yack/type/authority.hpp"

namespace yack
{

    namespace jive
    {

        namespace syntax
        {

            class terminal;

            class expected : public object, public authority<const terminal>
            {
            public:
                typedef  cxx_list_of<expected> list;
                
                virtual ~expected() throw();

                expected(const terminal &) throw();

                expected *next;
                expected *prev;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(expected);
            };

        }

    }

}

#endif

