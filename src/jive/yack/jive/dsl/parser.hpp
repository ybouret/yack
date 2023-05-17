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

        //______________________________________________________________________
        //
        //
        //! Domain Specific Language parser
        //
        //______________________________________________________________________
        class dsl_parser : public parser
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const int  _jstring = 0; //!< hash code for jstring
            static const int  _rstring = 1; //!< hash code for rstring
            static const int  _bstring = 2; //!< hash code for bstring
            static const char _include[];   //!< "#include"
            

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit dsl_parser();
            virtual ~dsl_parser() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________


            //! parse and preprocess
            syntax::xnode *compile(module *);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(dsl_parser);
            const hashing::perfect str2type; //!< discriminate strings

            module *open_from(const lexeme &lxm); //!< open matching file
            syntax::xnode *compile_(module        *, size_t &nr);
            void           process_(syntax::xnode *, size_t &nr);  //!< preprocess the directives
        };

    }


}

#endif

