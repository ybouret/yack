
#include "yack/aqueous/weasel/designer.hpp"
#include "yack/aqueous/weasel/parser.hpp"
#include "yack/aqueous/weasel/linker.hpp"

namespace yack
{
    namespace aqueous
    {

        namespace weasel
        {

            const char * const designer::edb[] =
            {
#include "db.hxx"
            };

            const size_t designer:: ndb = sizeof(edb) / sizeof(edb[0]);

            class designer::compiler : public parser, public linker
            {
            public:
                explicit compiler() : parser(), linker() {}
                virtual ~compiler() noexcept {}


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(compiler);
            };


            static void *workspace[ YACK_WORDS_FOR(designer::compiler) ];

            static inline
            designer::compiler *create_compiler()
            {
                return new (out_of_reach::address(out_of_reach::naught(workspace))) designer::compiler();
            }


            const char designer::call_sign[] = "weasel::compiler";

            designer:: designer() : impl( create_compiler() )
            {

            }

            designer:: ~designer() noexcept
            {
                out_of_reach::naught( destructed( static_cast<compiler *>(out_of_reach::address(workspace) ) ) );
            }

            void designer:: operator()(library        &lib,
                                       lua_equilibria &eqs,
                                       jive::module   *inp)
            {
                assert(NULL!=inp);
                jive::source    src(inp);
                auto_ptr<xnode> tree = impl->parse(src); if(tree.is_empty()) return;
                xnode          *root = & *tree;
                linker::params  data = { lib, eqs };
                linker::simplify(root);
                impl->walk(*root, &data);
            }

        }
    }

}

