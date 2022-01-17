
#include "yack/ios/bz/istream.hpp"
#include "yack/ios/bz/ostream.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/hashing/sha1.hpp"
#include "yack/hashing/md.hpp"
#include "yack/kr/digest.hpp"

using namespace yack;

namespace
{
    
}

YACK_UTEST(ios_bz)
{
    randomized::rand_ ran;

    hashing::sha1     H;
    if(argc>1)
    {
        const string     fn = "out.dat.bz2";
        H.set();
        {
            ios::icstream    src(argv[1]);
            ios::bz::ostream tgt(fn);

            size_t nr = 0;
            size_t buf[256];
            while( (nr=src.fetch(buf,ran.in(1,256))) > 0 )
            {
                H.run(buf,nr);
                tgt.frame(buf,nr);
            }
        }
        const digest md_src = hashing::md::of(H);
        std::cerr << "md=" << md_src << std::endl;

        {
            H.set();
            ios::bz::istream src(fn);
            size_t nr = 0;
            size_t buf[256];
            while( (nr=src.fetch(buf,ran.in(1,256))) > 0 )
            {
                H.run(buf,nr);
            }
        }
        const digest md_tgt = hashing::md::of(H);
        std::cerr << "md=" << md_tgt << std::endl;
        YACK_CHECK(md_src==md_tgt);

    }
}
YACK_UDONE()

