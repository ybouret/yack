
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
            //! setup with lexer/grammar name
            template <typename ID>
            explicit parser(const ID &id) :
            lexer(id), syntax::grammar(label)
            {
            }

            //! cleanup
            virtual ~parser() throw();

            //__________________________________________________________________
            //
            // terminal(s)
            //__________________________________________________________________

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

            //! load plugin to produce terminal 'id'
            template <typename PLUGIN>
            const rule &load(const string &id)
            {
                return term__(plug( type2type<PLUGIN>(), id).label,syntax::standard);
            }

            //! load plugin to produce terminal 'id'
            template <typename PLUGIN>
            const rule &load(const char *id)
            {
                return term__(plug( type2type<PLUGIN>(), id).label,syntax::standard);
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

