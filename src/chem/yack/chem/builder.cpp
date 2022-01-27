
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

            class Parser : public parser
            {
            public:
                inline virtual ~Parser() throw()
                {
                }

                inline explicit Parser() : parser("chemical::parser")
                {

                    compound   &COMPONENTS  = act("components");

                    const rule &POS1     = term("pos1",'+');
                    const rule &NEG1     = term("neg1",'-');
                    const rule &COEF     = term("coef","[1-9][0-9]*");
                    const rule &POS_COEF = agg("+coef") << POS1 << COEF;
                    const rule &NEG_COEF = agg("-coef") << NEG1 << COEF;

                    const rule &COEF1 = alt("coef1") << COEF << POS_COEF << NEG_COEF << POS1 << NEG1;
                    const rule &COEFX = alt("coefx") << POS_COEF << NEG_COEF << POS1 << NEG1;

                    compound   &SPECIES = agg("species");
                    {
                        SPECIES << mark('[');
                        SPECIES << term("name","[:alpha:][:word:]*");
                        compound   &CHARGES  = alt("charges");
                        CHARGES << term("posx","\\x2B{2}");
                        CHARGES << POS1;
                        CHARGES << term("negx","\\x2D{2}");
                        CHARGES << NEG1;
                        SPECIES << opt( CHARGES );
                        SPECIES << mark(']');
                    }

                    COMPONENTS << opt(COEF1) << SPECIES;

                    COMPONENTS << zom( cat(COEFX,SPECIES) );






                    gv();
                    validate();

                    drop("[:blank:]");
                }



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(Parser);

            };


        }
        
        const char builder:: call_sign[] = "chemical::builder";
        
        builder:: ~builder() throw()
        {
        }


        static const char *kw[] =
        {
            "pos1",
            "posx",
            "neg1",
            "negx",
            "-coef",
            "+coef",
            "species",
            "components",
            "coef"
        };


#define YACK_CHEM_POS1 0
#define YACK_CHEM_POSX 1
#define YACK_CHEM_NEG1 2
#define YACK_CHEM_NEGX 3
#define YACK_CHEM_NCOF 4
#define YACK_CHEM_PCOF 5
#define YACK_CHEM_SPEC 6
#define YACK_CHEM_COMP 7
#define YACK_CHEM_COEF 8



        builder:: builder() :
        P( new Parser() ),
        H( YACK_HASHING_PERFECT(kw) )
        {
            
        }
        
        builder::xnode * builder:: ast(const string &expr)
        {
            P->top()->verbose = true;
            P->reset();
            auto_ptr<syntax::xnode> ast = P->parse( module::open_data(expr) );
            if(ast.is_empty()) throw exception("%s: corrupted %s",call_sign,(*(P->label))());
            
            //ast->gv("components.dot");
            if(true)
            {
                ast->gv("chem.dot");
                jive::syntax::translator tr;
                tr.walk(*ast);
            }

            return ast.yield();
        }

        typedef jive::syntax::xlist xlist;

        species * builder:: ast_to_species(const xnode &tree) const
        {
            static const char  fn[] = "ast_to_species";
            static const char *spid = kw[YACK_CHEM_SPEC];

            const jive::syntax::rule &self      = *tree;
            const string             &self_name = *self.name;

            if(spid != self_name)
            {
                throw exception("%s::%s: unexpected '%s'",call_sign,fn,self_name());
            }

            const xnode *node = tree.head();
            string       name = node->word().to_string();
            unit_t       z    = 0;
            if( NULL !=( node=node->next) )
            {
                const string & zid = node->name();
                switch(H(zid))
                {
                    case YACK_CHEM_POS1: z= 1;            name += '+'; break;
                    case YACK_CHEM_NEG1: z=-1;            name += '-'; break;
                    case YACK_CHEM_POSX: z= node->ssize(); for(unit_t i=z;i>0;--i) name += '+'; break;
                    case YACK_CHEM_NEGX: z=-node->ssize(); for(unit_t i=z;i<0;++i) name += '-'; break;

                    default:
                        throw exception("%s::%s: invalid sign '%s'",call_sign,fn,zid());
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
#include "yack/ios/ascii/convert.hpp"

namespace yack
{

    namespace chemical
    {

        static inline unit_t node_to_unit(const jive::syntax::xnode *node)
        {
            assert("coef"==node->name());
            const string coef = node->word().to_string();
            return ios::ascii::convert::to<unit_t>(coef,"coefficient");

        }

        static inline unit_t node_to_coef(const jive::syntax::xnode *node,
                                          const hashing::perfect    &hash)
        {
            assert(node);
            const string &id = node->name();
            switch( hash(id) )
            {
                case YACK_CHEM_POS1: return  1;
                case YACK_CHEM_NEG1: return -1;
                case YACK_CHEM_PCOF: return  node_to_unit(node->tail());
                case YACK_CHEM_NCOF: return -node_to_unit(node->tail());
                case YACK_CHEM_COEF: return  node_to_unit(node);
                default:
                    break;
            }
            throw exception("%s: invalid coef node '%s'", builder::call_sign, id() );
        }



        void builder:: compile(components   &cmps,
                               const string &expr,
                               library      &lib)
        {
            static const char  fn[] = "compile";
            static const char *spid = kw[YACK_CHEM_SPEC];

            //------------------------------------------------------------------
            //
            //
            // check top-level status
            //
            //
            //------------------------------------------------------------------
            const auto_ptr<xnode> tree = ast(expr);
            const string         &kind = tree->name();

            switch(H(kind))
            {
                case YACK_CHEM_SPEC: {
                    const species::pointer sp = ast_to_species(*tree);
                    cmps.create(lib.check(sp),1);
                } return;

                case YACK_CHEM_COMP:
                    break;

                default:
                    throw exception("%s::%s invalid tree kind '%s'",call_sign,fn,kind());
            }

            //------------------------------------------------------------------
            //
            //
            // parse components
            //
            //
            //------------------------------------------------------------------
            const xnode *node = tree->head();
            unit_t       nu   = 1;

            //------------------------------------------------------------------
            //
            // optional first coef
            //
            //------------------------------------------------------------------
            if(node->name()!=spid)
            {
                nu = node_to_coef(node,H);
                node=node->next;
            }

            //------------------------------------------------------------------
            //
            // first species, mandatory
            //
            //------------------------------------------------------------------
            assert(node);
            assert(spid==node->name());
            {
                const species::pointer sp = ast_to_species(*node);
                cmps.create(lib.check(sp),nu);
            }

            //------------------------------------------------------------------
            //
            // extra species
            //
            //------------------------------------------------------------------
            node=node->next;
            while(node)
            {
                nu   = node_to_coef(node,H);
                node = node->next; assert(node);
                const species::pointer sp = ast_to_species(*node);
                cmps.create(lib.check(sp),nu);
                node = node->next;
            }


        }

    }
    
}
