
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

            class translator
            {
            public:
                explicit translator() throw();
                virtual ~translator() throw();

                virtual void on_terminal(const string &, const token&);
                virtual void on_internal(const string &, const size_t);
                void         walk(const xnode &tree);


            protected:
                unsigned depth;

                std::ostream & indent(std::ostream &) const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(translator);
                void inspect(const xnode *node);
                
            };
        }

    }

}

#endif
