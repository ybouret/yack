
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

                    compound &EQUILIBRIUM = act("equilibrium");

                    //__________________________________________________________
                    //
                    // define a species
                    //__________________________________________________________
                    compound   &SPECIES = agg("species");
                    {
                        SPECIES << term("name","[:upper:][:word:]*");
                        compound   &CHARGES  = alt("charges");
                        CHARGES << term("positive","\\x2B+");
                        CHARGES << term("negative","\\x2D+");
                        SPECIES << opt( CHARGES );
                    }

                    EQUILIBRIUM << SPECIES;


                    gv();
                    drop("[:blank:]");
                    validate();
                }

                inline syntax::xnode *compile(const string &expr)
                {
                    reset();
                    source         src( module::open_data(expr) );
                    syntax::xnode *res = (*this)(src);
                    if(!res) throw exception("%s: corrupted %s", builder::call_sign, (*label)() );
                    return res;
                }

                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(eq_parser);
            };
        }
        
        const char builder:: call_sign[] = "chemical::builder";
        
        builder:: ~builder() throw()
        {
        }
        
        builder:: builder() :
        eq( new eq_parser() )
        {
            
        }
        
        species * builder:: parse_species(const string &expr)
        {
            eq->reset();
            source                              src( module::open_data(expr) );
            const auto_ptr<const syntax::xnode> ast = (*eq)(src);
            if(ast.is_empty()) throw exception("%s: corrupted %s",call_sign,(*(eq->label))());
            
            ast->gv("eq.dot");
            {
                jive::syntax::translator tr;
                tr.walk(*ast);
            }

#if 0
            const jive::syntax::xnode *node = ast->head();
            string                     name = node->word().to_string();
            unit_t                     z    = 0;
            if( NULL != (node=node->next) )
            {
                const string &id = *(**node).name;
                const size_t  nz = node->size();
                char          s  = 0;
                if( "negative" == id)
                {
                    z = -unit_t(nz);
                    s = '-';
                }
                if( "positive" == id)
                {
                    z = unit_t(nz);
                    s = '+';
                }
                for(size_t i=nz;i>0;--i)
                    name += s;
            }
            return new species(name,z);
#endif
            exit(1);
            return NULL;
        }
        
        
    }
    
}
