#include "yack/chem/weasel/designer.hpp"
#include "yack/chem/weasel/parser.hpp"
#include "yack/chem/weasel/linker.hpp"

namespace yack
{
    namespace Chemical
    {

        namespace Weasel
        {
            static void *parser_wksp[ YACK_WORDS_FOR(Parser) ];
            static void *linker_wksp[ YACK_WORDS_FOR(Linker) ];

            
            void Designer:: cleanup() noexcept
            {
                if(parser)
                {
                    out_of_reach::zset( destructed(parser), sizeof(parser_wksp) );
                    parser = 0;
                }

                if(linker)
                {
                    out_of_reach::zset( destructed(linker), sizeof(linker_wksp) );
                    linker = 0;
                }

            }

            Designer:: ~Designer() noexcept
            {
                cleanup();
            }
            
            Designer:: Designer() : singleton<Designer>(), parser(0), linker(0)
            {
                parser = new( YACK_STATIC_ZSET(parser_wksp) ) Parser();
                try
                {
                    linker = new( YACK_STATIC_ZSET(linker_wksp) ) Linker();
                }
                catch(...)
                {
                    cleanup();
                    throw;
                }
            }
            
            const char * const Designer:: call_sign = "Chemical::Designer";


            void Designer:: operator()(jive::module *m, Library &lib, LuaEquilibria &eqs)
            {

                auto_ptr<XNode> tree = (*parser)(m);
                if(tree.is_valid())
                {
                    (*linker)(*tree,lib,eqs);
                    aliases(lib,eqs);
                }
                eqs.finalize();
            }

        }

    }

}

