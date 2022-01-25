
#include "yack/chem/builder.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/translator.hpp"
#include "yack/exception.hpp"

namespace yack
{
    
    namespace chemical
    {
        using namespace jive;

        namespace
        {
            class cm_parser : public  parser
            {
            public:
                inline virtual ~cm_parser() throw()
                {
                }

                
                inline explicit cm_parser() : parser("chemical::parser")
                {


                    compound   &COMPONENTS  = act("components");
                    const rule &COEFFICIENT = term("coefficient","[1-9][0-9]*");
                    const rule &POS_SOLO    = term("pos_solo",'+');
                    const rule &NEG_SOLO    = term("neg_solo",'-');
                    const rule &SIGN        = alt("sign") << POS_SOLO << NEG_SOLO;
                    const rule &OPT_COEFF   = opt(COEFFICIENT);

                    compound &FIRST_COEFF   = agg("first_coeff") << opt(SIGN) << OPT_COEFF;


                    //__________________________________________________________
                    //
                    // define an optional first coefficient
                    //__________________________________________________________
                    COMPONENTS << FIRST_COEFF;

                    //__________________________________________________________
                    //
                    // define a species
                    //__________________________________________________________
                    compound   &SPECIES = agg("species");
                    {
                        SPECIES << mark('[');
                        SPECIES << term("name","[:upper:][:word:]*");
                        compound   &CHARGES  = alt("charges");
                        CHARGES << term("pos_many","\\x2B{2}");
                        CHARGES << POS_SOLO;
                        CHARGES << term("neg_many","\\x2D{2}");
                        CHARGES << NEG_SOLO;
                        SPECIES << opt( CHARGES );
                        SPECIES << mark(']');
                    }

                    //__________________________________________________________
                    //
                    // define optional extra species
                    //__________________________________________________________
                    COMPONENTS << SPECIES;
                    compound &EXTRA_COEFF = agg("extra_coeff") << SIGN << OPT_COEFF;
                    COMPONENTS << zom( cat(EXTRA_COEFF,SPECIES) );

                    gv();
                    drop("[:blank:]");
                    validate();
                }


                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(cm_parser);
            };
        }
        
        const char builder:: call_sign[] = "chemical::builder";
        
        builder:: ~builder() throw()
        {
        }


        static const char *zkw[] =
        {
            "pos_solo",
            "pos_many",
            "neg_solo",
            "neg_many"
        };


#define YACK_CHEM_POS_SOLO 0
#define YACK_CHEM_POS_MANY 1
#define YACK_CHEM_NEG_SOLO 2
#define YACK_CHEM_NEG_MANY 3

        static const char *tkw[] =
        {
            "species",
            "components"
        };

#define YACK_CHEM_SP 0
#define YACK_CHEM_CM 1


        builder:: builder() :
        cm( new cm_parser() ),
        zh( YACK_HASHING_PERFECT(zkw) ),
        th( YACK_HASHING_PERFECT(tkw) )
        {
            
        }
        
        builder::xnode * builder:: ast(const string &expr)
        {
            cm->top()->verbose = true;
            cm->reset();
            source                  src( module::open_data(expr) );
            auto_ptr<syntax::xnode> ast = cm->parse(src);
            if(ast.is_empty()) throw exception("%s: corrupted %s",call_sign,(*(cm->label))());
            
            ast->gv("components.dot");
            {
                jive::syntax::translator tr;
                tr.walk(*ast);
            }

            return ast.yield();
        }

        typedef jive::syntax::xlist xlist;

        species * builder:: ast_to_species(const xnode &tree) const
        {
            static const char fn[] = "ast_to_species";
            static const char *sp  = tkw[YACK_CHEM_SP];

            const jive::syntax::rule &self = *tree;
            const string             &self_name = *self.name;

            if(sp != self_name)
            {
                throw exception("%s::%s: unexpected '%s'",call_sign,fn,self_name());
            }

            const xnode *node = tree.head();
            string       name = node->word().to_string();
            unit_t       z    = 0;
            if( NULL !=( node=node->next) )
            {
                const string & zid = *(**node).name;
                switch(zh(zid))
                {
                    case YACK_CHEM_POS_SOLO: z=1;             name += '+'; break;
                    case YACK_CHEM_NEG_SOLO: z=-1;            name += '-'; break;
                    case YACK_CHEM_POS_MANY: z=node->ssize();  for(unit_t i=z;i>0;--i) name += '+'; break;
                    case YACK_CHEM_NEG_MANY: z=-node->ssize(); for(unit_t i=z;i<0;++i) name += '-'; break;

                    default:
                        throw exception("%s::%s: unknown token '%s'",call_sign,fn,zid());
                }
            }
            return new species(name,z);
        }

        species * builder:: compile(const string &expr)
        {
            const auto_ptr<xnode> tree = ast(expr);
            return ast_to_species(*tree);
        }

    }

}

#include "yack/chem/library.hpp"
namespace yack
{

    namespace chemical
    {

        void builder:: compile(components   &cmps,
                               const string &expr,
                               library      &lib)
        {
            static const char fn[] = "compile";
            const auto_ptr<xnode> tree = ast(expr);
            const string         &kind = *(**tree).name;

            switch(th(kind))
            {
                case YACK_CHEM_SP: {
                    std::cerr << "simple species" << std::endl;
                    const species::pointer sp = ast_to_species(*tree);
                    if(!cmps.add(lib.check(sp),1))
                    {

                    }
                } break;

                case YACK_CHEM_CM:
                    std::cerr << "components..." << std::endl;
                    break;

                default:
                    throw exception("%s::%s invalid tree kind '%s'",call_sign,fn,kind());
            }






        }

    }
    
}
