#include "yack/hashing/adler32.hpp"
#include "yack/hashing/bjh32.hpp"
#include "yack/hashing/crc16.hpp"
#include "yack/hashing/crc32.hpp"
#include "yack/hashing/elf.hpp"
#include "yack/hashing/fnv.hpp"
#include "yack/hashing/md2.hpp"
#include "yack/hashing/md4.hpp"
#include "yack/hashing/md5.hpp"
#include "yack/hashing/pjw.hpp"
#include "yack/hashing/rmd128.hpp"
#include "yack/hashing/rmd160.hpp"
#include "yack/hashing/sfh.hpp"
#include "yack/hashing/sha1.hpp"
#include "yack/hashing/sha256.hpp"
#include "yack/hashing/sha512.hpp"
#include "yack/hashing/md.hpp"
#include "yack/hashing/to.hpp"

#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/type/hexa.hpp"

#include "yack/kr/digest.hpp"

#include <cstring>

using namespace yack;

namespace  {

    static inline void test_hashing( hashing::function &h, const char *filename)
    {
        std::cerr << std::setw(10) << h.name();

        std::cerr << " | length = " <<  std::setw(3) << h.length;
        std::cerr << " | window = " <<  std::setw(3) << h.window;

        if(filename)
        {
            h.set();
            char          buffer[256];
            const size_t  buflen = sizeof(buffer);
            ios::icstream fp(filename);
            while(true)
            {
                const size_t nr = fp.fetch(buffer,buflen);
                if(!nr) break;
                h.run(buffer,nr);
            }
            const digest md = hashing::md::of(h);
            std::cerr << " | " << md;
        }

        std::cerr << std::endl;
    }

}


#define ADD_FUNC(NAME) do { \
YACK_SIZEOF(hashing::NAME);\
const hashing::function::pointer h( new hashing::NAME() );\
H.push_back(h); } while(false)

YACK_UTEST(hashing_fcn)
{

    vector<hashing::function::pointer> H(32,as_capacity);

    ADD_FUNC(adler32);
    ADD_FUNC(bjh32);
    ADD_FUNC(crc16);
    ADD_FUNC(crc32);
    ADD_FUNC(elf);
    ADD_FUNC(fnv);
    ADD_FUNC(md2);
    ADD_FUNC(md4);
    ADD_FUNC(md5);
    ADD_FUNC(pjw);
    ADD_FUNC(rmd128);
    ADD_FUNC(rmd160);
    ADD_FUNC(sfh);
    ADD_FUNC(sha1);
    ADD_FUNC(sha224);
    ADD_FUNC(sha256);
    ADD_FUNC(sha384);
    ADD_FUNC(sha512);

    hashing::md5::tests();
    hashing::sha1::tests();

    for(size_t i=1;i<=H.size();++i)
    {
        hashing::function &hfn = *H[i];
        test_hashing(hfn,argv[1]);
    }

    hashing::to_key<uint16_t,hashing::md5> kh;

    const char data[] = "message digest";
    digest         src(data,strlen(data));
    const int      tmp = 10;
    const uint16_t k1  = kh(src);
    const uint16_t k2  = kh(tmp);
    std::cerr << "k1=" << k1 << std::endl;
    std::cerr << "k2=" << k2 << std::endl;


    


}
YACK_UDONE()
