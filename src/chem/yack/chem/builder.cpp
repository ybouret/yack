
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
                    compound &SPECIES = agg("species");
                    SPECIES << term("name","[:upper:][:word:]*");
                    compound &CHARGES = alt("charges");
                    CHARGES << oom( term('+') ) << oom( term('-') );
                    SPECIES << opt( CHARGES );
                    //gv();
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
            
            const jive::syntax::xlist &content = ast->sub();
            const jive::syntax::xnode *node    = content.head;
            string                     name    = node->word().to_string();
            const size_t               nsgn    = content.size-1;
            unit_t                     z       = 0;
            if(nsgn)
            {
                char s = '+';
                node=node->next;
                if('-'==*((**node).name))
                {
                    s='-';
                    z=-z;
                }
                for(size_t i=nsgn;i>0;--i) name += s;
            }
            return new species(name,z);
        }
        
        
    }
    
}
