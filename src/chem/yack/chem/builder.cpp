
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
                    const rule &COEF        = term("coef","[1-9][0-9]*");
                    const rule &POS_SOLO    = term("pos1",'+');
                    const rule &NEG_SOLO    = term("neg1",'-');
                    const rule &SIGN        = alt("sign") << POS_SOLO << NEG_SOLO;
                    const rule &OPT_COEF    = opt(COEF);
                    compound &FIRST_COEF    = agg("coef1") << opt(SIGN) << OPT_COEF;


                    //__________________________________________________________
                    //
                    // define an optional first coefficient
                    //__________________________________________________________
                    COMPONENTS << FIRST_COEF;

                    //__________________________________________________________
                    //
                    // define a species
                    //__________________________________________________________
                    compound   &SPECIES = agg("species");
                    {
                        SPECIES << mark('[');
                        SPECIES << term("name","[:upper:][:word:]*");
                        compound   &CHARGES  = alt("charges");
                        CHARGES << term("posx","\\x2B{2}");
                        CHARGES << POS_SOLO;
                        CHARGES << term("negx","\\x2D{2}");
                        CHARGES << NEG_SOLO;
                        SPECIES << opt( CHARGES );
                        SPECIES << mark(']');
                    }

                    //__________________________________________________________
                    //
                    // define optional extra species
                    //__________________________________________________________
                    COMPONENTS << SPECIES;
                    compound &EXTRA_COEF = agg("coefx") << SIGN << OPT_COEF;
                    COMPONENTS << zom( cat(EXTRA_COEF,SPECIES) );

                    //gv();
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


        static const char *kw[] =
        {
            "pos1",
            "posx",
            "neg1",
            "negx",
            "coef",
            "species",
            "components"
        };


#define YACK_CHEM_POS_SOLO 0
#define YACK_CHEM_POS_MANY 1
#define YACK_CHEM_NEG_SOLO 2
#define YACK_CHEM_NEG_MANY 3
#define YACK_CHEM_COEF_VAL 4
#define YACK_CHEM_SP       5
#define YACK_CHEM_CM       6



        builder:: builder() :
        cm( new cm_parser() ),
        H( YACK_HASHING_PERFECT(kw) )
        {
            
        }
        
        builder::xnode * builder:: ast(const string &expr)
        {
            //cm->top()->verbose = true;
            cm->reset();
            source                  src( module::open_data(expr) );
            auto_ptr<syntax::xnode> ast = cm->parse(src);
            if(ast.is_empty()) throw exception("%s: corrupted %s",call_sign,(*(cm->label))());
            
            //ast->gv("components.dot");
            if(false)
            {
                jive::syntax::translator tr;
                tr.walk(*ast);
            }

            return ast.yield();
        }

        typedef jive::syntax::xlist xlist;

        species * builder:: ast_to_species(const xnode &tree) const
        {
            static const char  fn[] = "ast_to_species";
            static const char *spid = kw[YACK_CHEM_SP];

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
                const string & zid = *(**node).name;
                switch(H(zid))
                {
                    case YACK_CHEM_POS_SOLO: z= 1;            name += '+'; break;
                    case YACK_CHEM_NEG_SOLO: z=-1;            name += '-'; break;
                    case YACK_CHEM_POS_MANY: z= node->ssize(); for(unit_t i=z;i>0;--i) name += '+'; break;
                    case YACK_CHEM_NEG_MANY: z=-node->ssize(); for(unit_t i=z;i<0;++i) name += '-'; break;

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

        static inline unit_t node_to_coef(const jive::syntax::xnode *node)
        {
            assert(node);
            const string value = node->word().to_string();
            return ios::ascii::convert::to<unit_t>(value,"coefficient");
        }

        static inline unit_t get_first_coeff1(const jive::syntax::xnode *node,
                                              const hashing::perfect    &ch)
        {
            assert(node);
            const string &id = node->name();
            switch( ch(id) )
            {
                case YACK_CHEM_POS_SOLO: return  1;
                case YACK_CHEM_NEG_SOLO: return -1;
                case YACK_CHEM_COEF_VAL: return node_to_coef(node);
                default:
                    break;
            }
            throw  exception("%s: unhandled first coefficient '%s'", builder::call_sign,id() );
        }

        static inline unit_t get_first_coeff2(const jive::syntax::xnode *head,
                                              const jive::syntax::xnode *tail,
                                              const hashing::perfect    &ch)
        {
            assert(head);
            assert(tail);
            const string &sid = head->name();
            unit_t        res = 0;
            switch( ch(sid) )
            {
                case YACK_CHEM_POS_SOLO: res=1;  break;
                case YACK_CHEM_NEG_SOLO: res=-1; break;
                default:
                    throw  exception("%s: unhandled coefficient sign '%s'", builder::call_sign,sid() );
            }
            return res * node_to_coef(tail);
        }



        static inline unit_t get_first_coeff(const jive::syntax::xnode *coef,
                                             const hashing::perfect    &H)
        {
            assert(NULL!=coef);
            const jive::syntax::xlist &cl   = coef->sub();
            switch(cl.size)
            {
                case 1: return get_first_coeff1(cl.head,H);
                case 2: return get_first_coeff2(cl.head,cl.tail,H);
                default:
                    break;
            }
            throw exception("%s: corrupted first coefficient syntax", builder::call_sign);
        }


        static inline unit_t get_extra_coeff1(const jive::syntax::xnode *node,
                                              const hashing::perfect    &H)
        {
            const string &sid = node->name();
            switch( H(sid) )
            {
                case YACK_CHEM_POS_SOLO: return 1;
                case YACK_CHEM_NEG_SOLO: return -1;
                default:
                    break;
            }
            throw  exception("%s: unhandled extra coef sign '%s'", builder::call_sign,sid() );
        }



        static inline unit_t get_extra_coeff(const jive::syntax::xnode *coef,
                                             const hashing::perfect    &H)
        {
            assert(NULL!=coef);
            const jive::syntax::xlist &cl = coef->sub();
            switch(cl.size)
            {
                case 1: return get_extra_coeff1(cl.head,H);
                case 2: return get_first_coeff2(cl.head,cl.tail,H);
                default:
                    break;
            }
            throw exception("%s: corrupted extra coefficient syntax", builder::call_sign);
        }



        void builder:: compile(components   &cmps,
                               const string &expr,
                               library      &lib)
        {
            static const char     fn[] = "compile";

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
                case YACK_CHEM_SP: {
                    const species::pointer sp = ast_to_species(*tree);
                    cmps.create(lib.check(sp),1);
                } break;

                case YACK_CHEM_CM:
                    break;

                default:
                    throw exception("%s::%s invalid tree kind '%s'",call_sign,fn,kind());
            }

            //------------------------------------------------------------------
            //
            //
            // loop over components
            //
            //
            //------------------------------------------------------------------
            assert(tree->size()>0);
            unit_t       coef = 1;
            size_t       left = tree->size(); assert(left>0);
            const xnode *node = tree->head(); assert(node);

            //------------------------------------------------------------------
            //
            // get first_coeff, optional
            //
            //------------------------------------------------------------------
            if( "coef1" == node->name())
            {
                coef = get_first_coeff(node,H);
                node = node->next;
                --left;
            }

            //------------------------------------------------------------------
            //
            // get first species, mandatory
            //
            //------------------------------------------------------------------
            assert(node!=NULL); assert(left>0); assert("species"==node->name());
            {
                const species::pointer sp = ast_to_species(*node);
                cmps.create(lib.check(sp),coef);
            }

            //------------------------------------------------------------------
            //
            // extra components
            //
            //------------------------------------------------------------------
            node=node->next;
            --left;
            assert( 0 == (left&1) );
            while(node)
            {
                assert(left>0);
                assert("coefx"==node->name());
                //--------------------------------------------------------------
                // get extra_coeff
                //--------------------------------------------------------------
                coef = get_extra_coeff(node,H);
                node = node->next; --left; assert(node); assert(left>0);

                //--------------------------------------------------------------
                // get species
                //--------------------------------------------------------------
                const species::pointer sp = ast_to_species(*node);
                cmps.create(lib.check(sp),coef);
                node=node->next; --left;
            }

            assert(NULL==node);
            assert(0   ==left);

        }

    }
    
}
