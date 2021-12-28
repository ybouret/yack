
//! \file

#ifndef YACK_JIVE_PARSER_INCLUDED
#define YACK_JIVE_PARSER_INCLUDED 1

#include "yack/jive/syntax/grammar.hpp"


namespace yack
{
    namespace jive
    {
        
        class parser : public lexer, public syntax::grammar
        {
        public:
            template <typename ID>
            explicit parser(const ID &id) :
            lexer(id),
            syntax::grammar(label)
            {
            }
            
            virtual ~parser() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(parser);
        };
    }
}

#endif

