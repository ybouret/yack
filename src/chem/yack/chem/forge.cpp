
#include "yack/chem/forge.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/exception.hpp"

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
                    const rule &SCOEF = agg("scoef") << S << COEF;
                    COEF1      << choice(SCOEF, S, COEF);
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

        
        static const char *kw[] =
        {
            "+",     // 0
            "-",     // 1
            "++",    // 2
            "--",    // 3
            "sp",    // 4
            "id",    // 5
            "coef",  // 6
            "coef1", // 7
            "scoef", // 8
            "xcoef"  // 9
        };
        
#define YCF_P     0
#define YCF_M     1
#define YCF_XP    2
#define YCF_XM    3
#define YCF_SP    4
#define YCF_ID    5
#define YCF_COEF  6
#define YCF_COEF1 7
#define YCF_SCOEF 8
#define YCF_XCOEF 9

        
        forge:: forge() :
        compiler( new cm_parser() ),
        keywords( YACK_HASHING_PERFECT(kw) )
        {
            std::cerr << "sizeof(parser)=" << sizeof(cm_parser) << std::endl;
            
            ios::ocstream fp(ios::cstderr);
            compiler->emit_keywords(fp);
        }

        typedef jive::syntax::xnode XNode;

        static inline int coef_to_int(const XNode *node)
        {
            assert(node);
            assert("coef"==node->name());
            
            return 0;
        }
        
        static inline int scoef_to_int(const XNode *node)
        {
            assert(node);
            assert("scoef"==node->name());
            
            return 0;
        }
        
        static inline int coef1_to_int(const XNode            *node,
                                       const hashing::perfect &hash)
        {
            assert(node);
            assert("coef1"==node->name());
            const XNode *scan = node->head();
            std::cerr << "[scan=" << scan->name() << "]" << std::endl;
            const string &name = scan->name();
            switch(hash(name))
            {
                case YCF_P:     return  1;
                case YCF_M:     return -1;
                case YCF_COEF:  return coef_to_int(scan);
                case YCF_SCOEF:
                    break;
                default:
                    throw exception("invalid '%s'", name());
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
            int          coef = 1;
            if("coef1"==node->name())
            {
                coef=coef1_to_int(node,keywords);
                node=node->next;
            }
            assert(node);
            assert("sp"==node->name());


        }

    }

}
