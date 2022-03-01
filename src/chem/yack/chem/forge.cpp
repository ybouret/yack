
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
                    compound   &COMPONENTS = agg("cm");

                    const rule &P  = term('+');
                    const rule &M  = term('-');
                    const rule &S  = alt("+/-") << P << M;
                    const rule &XP = term("++","\\x2B+");
                    const rule &XM = term("--","\\x2D+");
                    const rule &XS = alt("++/--") << XP << XM;
                    compound   &SP = agg("sp");
                    SP << mark('[');
                    SP << term("id", "[:alpha:][:word:]*");
                    SP << opt( choice(S,XS) );
                    SP << mark(']');

                    const rule &COEF = term("coef","[1-9][0-9]*");
                    compound   &COEF1 = agg("coef1");
                    COEF1      << choice( cat(S,COEF), S, COEF);
                    COMPONENTS << opt(COEF1) << SP;

                    compound &XCOEF  = agg("xcoef") << S << opt(COEF);
                    COMPONENTS << zom( cat(XCOEF,SP) );


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

        typedef jive::syntax::xnode XNode;

        static inline int coef1_to_nu(const XNode *node) throw()
        {
            assert(node);
            assert("coef1"==node->name());
            switch(node->size())
            {
                case 1:
                    break;

                case 2:
                    break;
                    
                default:
                    break;
            }
            return 0;
        }

        void forge:: create(components   &cmp,
                           library       &lib,
                           jive::module  *m)
        {
            jive::source src(m);
            compiler->reset();
            const auto_ptr<XNode> tree = compiler->parse( src );
            assert(tree.is_valid());
            tree->gv("forge.dot");
            assert("cm"==tree->name());

            const XNode *node = tree->head(); assert(node);
            int          coef = 0;
            if("coef1"==node->name())
            {
                coef=coef1_to_nu(node);
                node=node->next;
            }
            assert(node);
            assert("sp"==node->name());


        }

    }

}
