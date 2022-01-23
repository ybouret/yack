
#include "yack/chem/builder.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/translator.hpp"
#include "yack/exception.hpp"

namespace yack
{
    
    namespace chemical
    {
        
        namespace
        {
            class sp_parser : public jive::parser
            {
            public:
                inline virtual ~sp_parser() throw()
                {
                }
                
                inline explicit sp_parser() : jive::parser("chemical::species")
                {
                    compound   &SPECIES = agg("species");
                    //const rule &COEFF   = term("coeff","-?[1-9][0-9]*");
                    SPECIES << term("name","[:upper:][:word:]*");
                    compound   &CHARGES  = alt("charges");
                    CHARGES << term("positive","\\x2B+");
                    CHARGES << term("negative","\\x2D+");
                    SPECIES << opt( CHARGES );
                    gv();
                    drop("[:blank:]");
                    validate();
                }
                
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sp_parser);
            };
        }
        
        const char builder:: call_sign[] = "chemical::builder";
        
        builder:: ~builder() throw()
        {
        }
        
        builder:: builder() :
        sp( new sp_parser() )
        {
            
        }
        
        species * builder:: parse_species(const string &expr)
        {
            //sp->top()->verbose = true;
            sp->reset();
            jive::source                              src( jive::module::open_data(expr) );
            const auto_ptr<const jive::syntax::xnode> ast = (*sp)(src);
            if(ast.is_empty()) throw exception("%s: corrupted %s",call_sign,(*(sp->label))());
            
            //ast->gv("species.dot");
            {
                jive::syntax::translator tr;
                tr.walk(*ast);
            }
            
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
        }
        
        
    }
    
}
