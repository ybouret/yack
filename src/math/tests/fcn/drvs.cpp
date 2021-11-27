#include "yack/math/fcn/gradient.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"
#include "yack/type/utils.hpp"
#include "../../../core/tests/main.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"

using namespace yack;
using namespace math;

namespace
{
    
    template <typename T>
    struct Func
    {
        size_t count;
        T  operator()(T x)
        {
            ++count;
            return cos(x);
        }
        
        
    };
    
    template <typename T>
    struct FuncN
    {
        size_t count;
        template <typename INPUT>
        T operator()(const INPUT &Z)
        {
            ++count;
            T sum = 0;
            for(size_t i=Z.size();i>0;--i)
            {
                sum += T(i) *  squared( Z[i] );
            }
            return sum;
        }
        
    };
    
    template <typename T>
    static inline  void test(randomized::bits &ran)
    {
        std::cerr << "drvs for " << rtti::name<T>() << std::endl;
        std::cerr << "   unit_step_size: " << derivative<T>::unit_step_size() << std::endl;
        std::cerr << "   diff_maxi_ftol: " << derivative<T>::diff_maxi_ftol() << std::endl;
        
        {
            Func<T>       F = { 0 };
            derivative<T> drvs;
            const T       h(1e-4);
            const string  filename = vformat("drvs%u.dat", unsigned(sizeof(T)*8) );
            ios::ocstream fp(filename);
            size_t        it = 0;
            for(T x=-10;x<=10;x+=T(0.01))
            {
                ++it;
                const T dF  = drvs.diff(F,x,h);
                const T f0  = F(x);
                fp("%Lg %Lg %Lg\n", (long double)x, (long double)f0, (long double)dF);
            }
            std::cerr << "count=" << F.count << " / points=" << it << std::endl;
        }
        
        {
            const typename derivative<T>::pointer drvs = new derivative<T>;
            gradient<T>  grad(drvs);
            FuncN<T>     F = { 0 };
            for(size_t iter=0;iter<16;++iter)
            {
                v2d<T>   Z2 = bring::get< v2d<T> >(ran);
                v2d<T>   Z0 = Z2;
                v2d<T>   G2;
                grad.compute(G2,F,Z2);
                YACK_ASSERT( (Z0-Z2).norm2() <= 0 );
                std::cerr << "grad@" << Z2 << " => " << G2 << std::endl;
            }
            
            for(size_t iter=0;iter<16;++iter)
            {
                v3d<T>   Z3 = bring::get< v3d<T> >(ran);
                v3d<T>   Z0 = Z3;
                v3d<T>   G3;
                grad.compute(G3,F,Z3);
                YACK_ASSERT( (Z0-Z3).norm2() <= 0 );
                std::cerr << "grad@" << Z3 << " => " << G3 << std::endl;
            }
            
            vector<T> Z(4,0);
            list<T>   G(4,0);
            for(size_t iter=0;iter<16;++iter)
            {
                bring::fill(Z,ran);
                grad.compute(G,F,Z);
                std::cerr << "grad@" << Z << " => " << G << std::endl;
            }
            
        }
        
        
        
    }
    
}

YACK_UTEST(drvs)
{
    randomized::rand_ ran;
    test<float>(ran);
    test<double>(ran);
    test<long double>(ran);
    
}
YACK_UDONE()


