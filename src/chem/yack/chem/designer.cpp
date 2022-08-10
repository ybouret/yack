
#include "yack/chem/designer.hpp"

namespace yack
{
    using namespace jive;

    namespace chemical
    {


        const char * const designer:: call_sign = "chemical::designer";

        designer:: ~designer() throw()
        {
        }


        namespace
        {
            class sp_build  : public syntax::translator
            {
            public:
                explicit sp_build() : syntax::translator(),
                name()
                {
                }

                virtual ~sp_build() throw() {}

                string name;
                string z;

                



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sp_build);
            };

            class sp_parser : public   parser
            {
            public:
                inline virtual ~sp_parser() throw() {}

                inline explicit sp_parser() : jive:: parser("chemical::species::parser")
                {
                    syntax::compound &SPECIES = agg("SPECIES");
                    SPECIES << term("NAME", "[:upper:][:word:]*");
                    {
                        const syntax::rule &POS = oom( term('+') );
                        const syntax::rule &NEG = oom( term('-') );
                        SPECIES << opt( choice(POS,NEG) );
                    }
                    //gv();
                }
                

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sp_parser);
            };
        }

        designer:: designer() :
        sp( new sp_parser() )
        {
        }


        void designer::species_from(jive::module *m)
        {
            source src(m);
            sp->reset();

            auto_ptr<syntax::xnode> spx  = sp->parse(src);
            sp_build                spt;
            spt.walk(*spx,NULL);

        }

    }

}
