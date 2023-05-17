
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
            // definition
            //__________________________________________________________________
            typedef ark_ptr<string,parser> handle; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup with lexer/grammar name
            template <typename ID>
            explicit parser(const ID &id) :
            lexer(id), syntax::grammar(label)
            {
            }

            //! cleanup
            virtual ~parser() noexcept;

            //__________________________________________________________________
            //
            // terminal(s)
            //__________________________________________________________________

            //! create a new terminal from a compiler pattern
            template <typename ID>
            const rule & user(const ID &id, pattern *p, const bool keep=true) {
                const motif             m = p;
                const tag               t = tags::make(id);
                const lexical::action   a(this,&scanner::on_produce);  // make a leexeme
                const syntax::term_role r = keep ? (m->is_univocal() ? syntax::univocal : syntax::standard) : syntax::division;
                return create(t,m,a,r);
            }

            //! create a new terminal id from expression rx
            template <typename ID, typename RX> inline
            const rule & term(const ID &id, const RX &rx)
            {
                const tag               t = tags::make(id);
                const motif             m = regexp::compile(rx,&dict);
                const lexical::action   a(this,&scanner::on_produce);
                const syntax::term_role r = m->is_univocal() ? syntax::univocal : syntax::standard;
                return create(t,m,a,r);
            }

            //! create a terminal whose regexp is id
            template <typename ID> inline
            const rule &term(const ID &id)
            {
                return term(id,id);
            }

            //! specific EOL: produce lexeme and update line count
            template <typename ID, typename RX> inline
            const rule & EOL(const ID &id, const RX &rx, const bool keep=false)
            {
                const tag               t = tags::make(id);
                const motif             m = regexp::compile(rx,&dict);
                const lexical::action   a(this,&scanner::on_endline);  // produce ENDL
                const syntax::term_role r = keep ? (m->is_univocal() ? syntax::univocal : syntax::standard) : syntax::division;
                return create(t,m,a,r);
            }

            //__________________________________________________________________
            //
            // division(s)
            //__________________________________________________________________

            //! create a division 'id' from expression 'rx'
            template <typename ID, typename RX> inline
            const rule & mark(const ID &id, const RX &rx)
            {
                const tag               t = tags::make(id);
                const motif             m = regexp::compile(rx,&dict);
                const lexical::action   a(this,&scanner::on_produce);
                on(t,m,a);
                return term__(t,syntax::division);
            }

            //! create a mark whose regexp is id
            template <typename ID> inline
            const rule &mark(const ID &id)
            {
                return mark(id,id);
            }


            //__________________________________________________________________
            //
            // no-args plugins to terminal
            //__________________________________________________________________

            //! load plugin (no-arg) to produce terminal 'uuid'
            template <
            typename PLUGIN,
            typename IDENTIFIER>
            const rule &load(const IDENTIFIER &uuid)
            {
                return term__(plug( type2type<PLUGIN>(), uuid).label,syntax::standard);
            }

            //! load plugin (1-arg) to produce terminal 'uuid'
            template <
            typename PLUGIN,
            typename IDENTIFIER,
            typename EXPRESSION>
            const rule &load(const IDENTIFIER &uuid, const EXPRESSION &expr)
            {
                return term__(plug( type2type<PLUGIN>(), uuid, expr).label,syntax::standard);
            }
            

            //__________________________________________________________________
            //
            // parse
            //__________________________________________________________________
            syntax::xnode * parse(source &); //!< parse source from current lexer status
            syntax::xnode * parse(module *); //!< parse module from current lexer status


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(parser);
            const rule &create(const tag &, const motif &, const lexical::action &, const syntax::term_role);
        };
    }
}

#endif

