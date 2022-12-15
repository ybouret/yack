
#include "yack/raven/qfamily.hpp"
#include "yack/raven/qselect.hpp"
#include "yack/utest/run.hpp"

#include "yack/sequence/vector.hpp"
#include "yack/container/matrix.hpp"
#include "yack/ios/ascii/convert.hpp"

#include "yack/counting/comb.hpp"
#include "yack/counting/perm.hpp"


using namespace yack;

namespace
{
    static inline void create(writable<int> &cf, randomized::bits &ran)
    {
        for(size_t i=cf.size();i>0;--i)
        {
            cf[i] = static_cast<int>( ran.in(-4,4) );
        }
    }

    static inline void create(matrix<int> &nu, randomized::bits &ran)
    {
        for(size_t i=nu.rows;i>0;--i)
        {
            create(nu[i],ran);
        }
    }

}


YACK_UTEST(raven_family)
{
    randomized::rand_ ran;

    {
        const size_t eqs = 2;
        const size_t spc = 3;

        matrix<int> nu(eqs,spc);
        create(nu,ran);
        matrix<int> mu;
        raven::qselect::compress(mu,nu);

        std::cerr << "nu=" << nu << std::endl;
        std::cerr << "mu=" << mu << std::endl;


    }


}
YACK_UDONE()

