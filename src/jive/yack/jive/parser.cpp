
#include "yack/jive/parser.hpp"
#include "yack/jive/pattern/basic/single.hpp"


namespace yack
{
    namespace jive
    {
        parser:: ~parser() throw()
        {
        }

        const syntax::rule &parser:: create(const tag &t, const motif &m, const lexical::action &a, const syntax::term_role r)
        {
            on(t,m,a);
            return term__(t,r);
        }

        const syntax::rule &parser:: term(const string &id)
        {
            return term(id,id);
        }

        const syntax::rule &parser:: term(const char *id)
        {
            return term(id,id);
        }

        const syntax::rule &parser:: term(const char C)
        {
            const tag             t = tags::make(C);
            const motif           m = new single(C);
            const lexical::action a(this, &scanner::on_produce);
            return create(t,m,a,syntax::univocal);
        }

        const syntax::rule &parser:: mark(const string &id)
        {
            return mark(id,id);
        }

        const syntax::rule &parser:: mark(const char *id)
        {
            return mark(id,id);
        }

        const syntax::rule &parser:: mark(const char C)
        {
            const tag             t = tags::make(C);
            const motif           m = new single(C);
            const lexical::action a(this, &scanner::on_produce);
            return create(t,m,a,syntax::division);
        }


    }
}

