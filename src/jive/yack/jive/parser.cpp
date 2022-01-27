
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


        syntax::xnode * parser:: parse(source &src)  
        {
            return run(src,*this);
        }

        syntax::xnode * parser::parse(module *m)
        {
            assert(m);
            source src(m);
            return parse(src);

        }

    }
}

