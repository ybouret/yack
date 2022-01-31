#include "yack/jive/pattern/matching.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/pattern/posix.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/fs/vfs.hpp"

using namespace yack;

YACK_UTEST(matching)
{

    if(argc>1)
    {
        const string   expr = argv[1];
        jive::matching match = expr;
        std::cerr << "express: " << match.scheme->express() << std::endl;;
        std::cerr << "strong : " << match.strong            << std::endl;;

        if(argc>2)
        {
            ios::icstream   fp(argv[2]);
            ios::characters line;
            unsigned        il = 0;
            const string    fn = vfs::get_base_name(argv[2]);
            while(fp.gets(line))
            {
                ++il;
                const string data = line.to_string();
                const string name = fn + vformat("%u",il);
                {
                    jive::source src( jive::module::open_data(name,data) );
                    if( match.exactly_(src) )
                    {
                        std::cerr << data << " [=> exactly] '" << match << "'" << std::endl;
                    }
                }

                {
                    jive::source src( jive::module::open_data(name,data) );
                    if( match.somehow_(src) )
                    {
                        std::cerr << data << " [=> somehow] '" << match << "'" << std::endl;
                    }
                }


            }
        }
    }

}
YACK_UDONE()

