
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
            class eq_parser : public  parser
            {
            public:
                inline virtual ~eq_parser() throw()
                {
                }



                inline explicit eq_parser() : parser("chemical::parser")
                {


                    compound   &EQUILIBRIUM = act("equilibrium");
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
                    EQUILIBRIUM << FIRST_COEFF;

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
                    EQUILIBRIUM << SPECIES;
                    compound &EXTRA_COEFF = agg("extra_coeff") << SIGN << OPT_COEFF;
                    EQUILIBRIUM << zom( cat(EXTRA_COEFF,SPECIES) );

                    gv();
                    drop("[:blank:]");
                    validate();
                }


                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(eq_parser);
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

        builder:: builder() :
        eq( new eq_parser() ),
        zh( YACK_HASHING_PERFECT(zkw) )
        {
            
        }
        
        builder::xnode * builder:: ast(const string &expr)
        {
            eq->top()->verbose = true;
            eq->reset();
            source                  src( module::open_data(expr) );
            auto_ptr<syntax::xnode> ast = eq->parse(src);
            if(ast.is_empty()) throw exception("%s: corrupted %s",call_sign,(*(eq->label))());
            
            ast->gv("eq.dot");
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
            const jive::syntax::rule &self = *tree;
            const string             &self_name = *self.name;
            if("species" != self_name)
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


        void builder:: compile(components &cmps, const string &expr, library &lib)
        {
            const auto_ptr<xnode> tree = ast(expr);
            

        }

    }
    
}
