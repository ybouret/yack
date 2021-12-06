#include "yack/associative/factory.hpp"
#include "yack/utest/run.hpp"
#include "yack/hashing/sha1.hpp"
#include "yack/hashing/rmd128.hpp"
#include "yack/string.hpp"
#include "yack/ptr/auto.hpp"

using namespace yack;

namespace
{

    template <typename FUNCTION> static inline
    hashing::function * create()
    {
        return new FUNCTION();
    }

}

YACK_UTEST(data_factory)
{
    typedef hashing::function * (*creator)();
    factory<hashing::function,string,creator> hfac;

    hfac.declare(hashing::sha1::clid,create<hashing::sha1>);

    const string id = hashing::rmd128::clid;
    hfac.declare(id,create<hashing::rmd128>);

    std::cerr << hfac << std::endl;

    YACK_CHECK(hfac.search("sha1"));
    YACK_CHECK(hfac.search("rmd128"));
    YACK_CHECK(hfac.search(hashing::sha1::clid));
    YACK_CHECK(hfac.search(hashing::rmd128::clid));
    YACK_CHECK(hfac.search(id));


    auto_ptr<hashing::function> H = hfac("sha1");
    std::cerr << "created <" << H->name() << ">" << std::endl;
    H = hfac("rmd128");
    std::cerr << "created <" << H->name() << ">" << std::endl;

}
YACK_UDONE()

