#include "yack/type/v3d.hpp"
#include "yack/utest/run.hpp"



using namespace yack;

namespace
{
    template <typename VTX> static inline
    void test_vtx( randomized::bits &ran)
    {
        typedef typename VTX::mutable_type type;

        VTX a;
        VTX b;

        for(size_t i=1;i<=a.size();++i)
        {
            a[i] = ran.to<type>();
            b[i] = ran.to<type>();
        }

        std::cerr << "a=" << a << ", b=" << b << std::endl;

        {
            VTX s1 = a+b;
            VTX s2 = a; s2 += b;
        }

        std::cerr << "|a|^2=" << a.norm2() << std::endl;
        
    }
}

YACK_UTEST(type_vtx)
{
    randomized::rand_ ran;

    test_vtx< v2d<float>  >(ran);
    test_vtx< v2d<double> >(ran);
    test_vtx< v3d<float>  >(ran);
    test_vtx< v3d<double> >(ran);


}
YACK_UDONE()

