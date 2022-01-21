
#include "yack/chem/builder.hpp"
#include "yack/jive/syntax/xnode.hpp"

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

                inline explicit sp_parser() : jive::parser("chemical::species::parser")
                {

                    
                    drop("[:blank:]");
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
            sp->reset();
            jive::source                              src( jive::module::open_data(expr) );
            const auto_ptr<const jive::syntax::xnode> ast = (*sp)(src);

            exit(1);
            return NULL;
        }


    }

}
