//! \file

#ifndef YACK_JIVE_DSL_PARSER_INCLUDED
#define YACK_JIVE_DSL_PARSER_INCLUDED 1

#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/hashing/perfect.hpp"

namespace yack
{
    namespace jive
    {

        class dsl_parser : public parser
        {
        public:
            static const int  _jstring = 0;
            static const int  _rstring = 1;
            static const int  _bstring = 2;
            static const char _include[];

            explicit dsl_parser();
            virtual ~dsl_parser() noexcept;

            //! include
            void preprocess(syntax::xnode *);



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(dsl_parser);
            const hashing::perfect str2type;
            module *open_from(const lexeme &lxm);
        };

    }


}

#endif

