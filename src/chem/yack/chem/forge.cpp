
#include "yack/chem/forge.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/hashing/perfect.hpp"
#include "yack/exception.hpp"
#include "yack/apex/natural.hpp"

namespace yack
{

    namespace chemical
    {

        namespace
        {

            static const char *tkw[] = {
                "+",
                "-",
                "X+",
                "X-",
                "id",
                "cf"
            };

#define YCF_x2B     0 //!< "+"
#define YCF_x2D     1 //!< "-"
#define YCF_Xx2B    2 //!< "X+"
#define YCF_Xx2D    3 //!< "X-"
#define YCF_id      4 //!< "id"
#define YCF_cf      5 //!< "cf"

            static const char *ikw[] = {
                "cm",
                "sp",
                "cf1",
                "scf",
                "xcf"
            };

#define YCF_cm     0 //!< "cm"
#define YCF_sp     1 //!< "sp"
#define YCF_cf1    2 //!< "cf1"
#define YCF_scf    3 //!< "scf"
#define YCF_xcf    4 //!< "xcf"


            struct cm_context
            {
                const char *who;
                components &cmp;
                library    &lib;
            };

            class cm_linker : public jive::syntax::translator
            {
            public:
                explicit cm_linker() :
                jive::syntax::translator(),
                thash( YACK_HASHING_PERFECT(tkw) ),
                ihash( YACK_HASHING_PERFECT(ikw) ),
                coefs(),
                names(),
                coef1()
                {
                }

                virtual ~cm_linker() throw()
                {
                }

                const hashing::perfect   thash;
                const hashing::perfect   ihash;

                vector<int>              coefs;
                vector<string>           names;
                bool                     coef1;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(cm_linker);
                inline void cleanup() throw()
                {
                    coefs.release();
                    names.release();
                    coef1=false;
                }
                virtual void on_init()
                {
                    cleanup();
                }

                virtual void on_quit() throw()
                {
                    cleanup();
                }

                virtual void on_terminal(const lexeme &lx)
                {
                    //jive::syntax::translator::on_terminal(lx);
                    const string &tid = *lx.name;
                    switch( thash(tid) )
                    {
                        case YCF_x2B:
                            coefs.push_back(1);
                            break;

                        case YCF_x2D:
                            coefs.push_back(-1);
                            break;

                        case YCF_Xx2B:
                            coefs.push_back( int(lx.data.size) );
                            break;

                        case YCF_Xx2D:
                            coefs.push_back( -int(lx.data.size) );
                            break;

                        case YCF_cf: {
                            static const char *which = tkw[YCF_cf];
                            const apn n = lx.data.to_apn(which);
                            coefs.push_back( n.cast_to<int>(which) );
                        } break;

                        case YCF_id: {
                            const string s = lx.data.to_string();
                            names.push_back(s);
                        } break;

                        default:
                            throw exception("%s unknown terminal '%s'",forge::call_sign,tid());
                    }

                }

                virtual void on_internal(const string &func, const size_t narg)
                {
                    //jive::syntax::translator::on_internal(func,narg);
                    assert(data);
                    switch( ihash(func) )
                    {
                        case YCF_scf: {
                            assert(coefs.size()>=2);
                            const int c = coefs.pop_back_value();
                            coefs.back() *= c;
                        } break;

                        case YCF_cf1: {
                            coef1=true;
                        } break;

                        case YCF_xcf: {
                            if(narg>1)
                            {
                                assert(coefs.size()>=2);
                                const int c = coefs.pop_back_value();
                                coefs.back() *= c;
                            }
                        } break;

                        case YCF_sp: {
                            assert(names.size()>0);
                            string name = names.pop_back_value();
                            int    z    = 0;
                            if(2==narg)
                            {
                                assert(coefs.size()>0);
                                z = coefs.pop_back_value();
                                const char c = (z<0) ? '-' : '+';
                                for(int i=absolute(z);i>0;--i) name += c;
                            }

                            cm_context    &ctx = *(cm_context*)data;
                            const species &sp  = ctx.lib(name,z);
                            int            nu  = 1;
                            if(coefs.size()>0)
                            {
                                nu=coefs.pop_back_value();
                            }
                            if(!ctx.cmp(sp,nu)) throw exception("%s has multiple %s",ctx.who,name());

                        } break;

                        case YCF_cm: {
                            
                        } break;

                        default:
                            throw exception("%s unknown internal '%s'/%u",forge::call_sign,func(), unsigned(narg));
                    }

                }
            };

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
                    const rule &XP = term("X+","\\x2B+");
                    const rule &XM = term("X-","\\x2D+");
                    const rule &XS = alt("++/--") << XP << XM;
                    compound   &SP = agg("sp");
                    SP << mark('[');
                    SP << term("id", "[:alpha:][:word:]*");
                    SP << opt( choice(S,XS) );
                    SP << mark(']');

                    const rule &COEF = term("cf","[1-9][0-9]*");
                    compound   &COEF1 = agg("cf1");
                    const rule &SCOEF = agg("scf") << S << COEF;
                    COEF1      << choice(SCOEF, S, COEF);
                    COMPONENTS << opt(COEF1) << SP;

                    compound &XCOEF  = agg("xcf") << S << opt(COEF);
                    COMPONENTS << zom( cat(XCOEF,SP) );


                    drop("[:blank:]");
                    endl("[:endl:]");


                    validate();
                }



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(cm_parser);
            };
        }

        const char * const forge:: call_sign = "chemical::forge";

        forge:: ~forge() throw() {}
        


        
        forge:: forge() :
        parser( new cm_parser() ),
        linker( new cm_linker() )
        {
            std::cerr << "sizeof(parser)=" << sizeof(cm_parser) << std::endl;

            if(false)
            {
                vector<string> terminals, internals;
                parser->collect_keywords(terminals,internals);
                std::cerr << "terminals=" << terminals << std::endl;
                std::cerr << "internals=" << internals << std::endl;

                ios::ocstream fp(ios::cstderr);
                jive::syntax::grammar::emit_keywords(fp,terminals,"tkw","YCF_");
                jive::syntax::grammar::emit_keywords(fp,internals,"ikw","YCF_");
            }

        }

        typedef jive::syntax::xnode XNode;


        void forge:: create(components   &cmp,
                            library       &lib,
                            jive::module  *m,
                            const char    *who)
        {
            jive::source src(m);
            parser->reset();
            const auto_ptr<XNode> tree = parser->parse( src );
            assert(tree.is_valid());
            cm_context ctx = { who ? who : yack_unknown, cmp, lib };
            linker->walk(*tree,&ctx);
        }


        const species & forge:: single(library &lib, jive::module *m)
        {
            jive::source src(m);
            parser->reset();
            const auto_ptr<XNode> tree = parser->parse( src );
            assert(tree.is_valid());

            const size_t nsub = tree->size(); assert(nsub>0);
            const XNode *node = tree->head(); assert(node!=NULL);
            if(nsub!=1 || node->name() != "sp" ) throw exception("%s not a single species",call_sign);

            components cmp;
            cm_context ctx = { "single", cmp, lib };
            linker->walk(*tree,&ctx);


            assert(1==cmp.size());
            const cnode   *cn = cmp.head();
            const species &sp = ****cn;
            return sp;
        }

        const species & library:: parse(const string &txt)
        {
            static forge &mgr = forge::instance();
            return mgr(*this,txt);
        }

        const species & library:: parse(const char *txt)
        {
            static forge &mgr = forge::instance();
            return mgr(*this,txt);
        }


    }

}

#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        void equilibrium:: parse_with(library &lib, const string &txt)
        {
            static forge &mgr = forge::instance();
            mgr(*this,lib,txt,name);
        }

    }

}



