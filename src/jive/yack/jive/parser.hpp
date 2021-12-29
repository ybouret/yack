
//! \file

#ifndef YACK_JIVE_PARSER_INCLUDED
#define YACK_JIVE_PARSER_INCLUDED 1

#include "yack/jive/syntax/grammar.hpp"


namespace yack
{
    namespace jive
    {


        //______________________________________________________________________
        //
        //
        //! a parser is a lexer with a grammar
        //
        //______________________________________________________________________
        class parser : public lexer, public syntax::grammar
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            template <typename ID>
            explicit parser(const ID &id) :
            lexer(id), syntax::grammar(label)
            {
            }
            
            virtual ~parser() throw();

            //__________________________________________________________________
            //
            // terminal(s)
            //__________________________________________________________________
            template <typename ID, typename RX> inline
            const rule & term(const ID &id, const RX &rx)
            {
                const tag               t = tags::make(id);
                const motif             m = regexp::compile(rx,&dict);
                const lexical::action   a(this,&scanner::on_produce);
                const syntax::term_role r = m->is_univocal() ? syntax::univocal : syntax::standard;
                on(t,m,a);
                term__(t,r);
            }

            template <typename ID> inline
            const rule & term(const ID &id)
            {
                return term(id,id);
            }

            //__________________________________________________________________
            //
            // division(s)
            //__________________________________________________________________
            template <typename ID, typename RX> inline
            const rule & mark(const ID &id, const RX &rx)
            {
                const tag               t = tags::make(id);
                const motif             m = regexp::compile(rx,&dict);
                const lexical::action   a(this,&scanner::on_produce);
                on(t,m,a);
                term__(t,syntax::division);
            }

            template <typename ID> inline
            const rule & mark(const ID &id)
            {
                return mark(id,id);
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(parser);
        };
    }
}

#endif

