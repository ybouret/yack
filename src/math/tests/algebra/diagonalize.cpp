#include "yack/math/algebra/diagonalize.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"
#include "../../../core/tests/main.hpp"

using namespace yack;

namespace
{

    template <typename T> static inline
    void test_diag(randomized::bits &ran)
    {
        std::cerr << "diag<" << rtti::name<T>() << ">" << std::endl;
        math::svd<T> s;
        for(size_t dim=1;dim<=8;++dim)
        {
            std::cerr << "dim=" << dim << std::endl;
            matrix<T> a(dim,dim);
            matrix<T> a0(dim,dim);
            vector<T> wr(dim);
            vector<T> wi(dim);
            vector<T> av(dim), lv(dim);
            size_t    nr = 0;

            for(size_t iter=0;iter<1;++iter)
            {
                for(size_t i=1;i<=dim;++i)
                {
                    for(size_t j=1;j<=dim;++j)
                    {
                        a[i][j] = ran.symm<T>();
                    }
                }
                a0.assign(a);

                // look for eigenvalues
                std::cerr << "a=" << a << std::endl;
                if(!math::diagonalize::eig(a,wr,wi,nr))
                {
                    std::cerr << "couldn't compute eigen values" << std::endl;
                    continue;
                }
                std::cerr << "nr=" << nr << std::endl;
                std::cerr << "wr=" << wr << std::endl;
                std::cerr << "wi=" << wi << std::endl;

                // look for eigenvectors
                if(nr>0)
                {
                    matrix<T> ev(nr,dim);
                    if(math::diagonalize::eigv(ev,a0,wr,s))
                    {
                        for(size_t i=1;i<=nr;++i)
                        {
                            const T            lam = wr[i];
                            const readable<T> &vec = ev[i];
                            std::cerr << "\twr" << i << "=" << lam << std::endl;
                            std::cerr << "\tev" << i << "=" << vec << std::endl;

                            a0(av,vec);
                            math::tao::v1::mul(lv,lam,vec);
                            T rms = sqrt(math::tao::v1::mod2<T>::of(av,lv)/dim);
                            std::cerr << "\t\trms" << i << "=" << rms << std::endl;
                        }
                    }
                    else
                    {
                        std::cerr << "couldn't find eigenvectors" << std::endl;
                    }
                }

            }

            
        }
        std::cerr << std::endl;
    }

}


YACK_UTEST(diagonalize)
{
    randomized::rand_ ran;

    test_diag<float>(ran);
    test_diag<double>(ran);
    test_diag<long double>(ran);

}
YACK_UDONE()


