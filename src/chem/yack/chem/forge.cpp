
#include "yack/chem/forge.hpp"
#include "yack/jive/syntax/xnode.hpp"

namespace yack
{

    namespace chemical
    {

        namespace
        {

            class cm_parser :   public jive::parser
            {
            public:
                inline virtual ~cm_parser() throw() {}

                inline explicit cm_parser() :
                jive::parser(forge::call_sign)
                {
                    compound   &COMPONENTS = agg("COMPONENTS");
                    const rule &P   = mark('+');
                    const rule &M   = term('-');
                    const rule &S   = alt("sign") << P << M;
                    const rule &PP  = term("++","\\x2B+");
                    const rule &MM  = term("--","\\x2D+");
                    compound   &SPECIES = agg("species");
                    SPECIES << mark('[');
                    SPECIES << term("root","[:alpha:][:word:]*");
                    SPECIES << opt( alt("charge") << S << PP << MM );
                    SPECIES << mark(']');

                    const rule &COEF       = term("coef","[1-9][0-9]*");
                    compound   &FIRST_COEF = agg("first_coef");
                    FIRST_COEF << choice( grp("signed_coef") << S << COEF,S,COEF);



                    COMPONENTS << opt(FIRST_COEF) << SPECIES;

                    const rule &EXTRA_COEF = agg("extra_coef") << S << opt(COEF);
                    COMPONENTS << zom( cat(EXTRA_COEF,SPECIES) );


                    drop("[:blank:]");
                    endl("[:endl:]");

                    gv();

                    validate();
                }



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(cm_parser);
            };
        }

        const char forge:: call_sign[] = "chemical::forge";

        forge:: ~forge() throw()
        {
        }

        forge:: forge() :
        compiler( new cm_parser() )
        {
            std::cerr << "sizeof(parser)=" << sizeof(cm_parser) << std::endl;
        }



        void forge:: create(components   &cmp,
                           library       &lib,
                           jive::module  *m)
        {
            jive::source src(m);
            compiler->reset();
            const auto_ptr<jive::syntax::xnode> tree = compiler->parse( src );
            tree->gv("forge.dot");
        }

    }

}
