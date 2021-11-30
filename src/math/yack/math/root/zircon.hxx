
#include "yack/sequence/vector.hpp"

namespace yack
{

    namespace math
    {

        template <>
        zircon<real_t>:: ~zircon() throw()
        {
        }


        template <>
        zircon<real_t>:: zircon(const size_t dims) :
        tableaux(4,dims),
        X(  next() ),
        F(  next() ),
        G(  next() ),
        VV( next() ),
        f0(0),
        J(dims,dims),
        Jt(dims,dims),
        iJ(dims,dims),
        fdjac(0)
        {
        }

        template <>
        void zircon<real_t>:: setup(size_t dims)
        {
            make(dims);
            J.make(dims,dims);
            Jt.make(dims,dims);
            iJ.make(dims,dims);
        }

        template <>
        real_t zircon<real_t>:: objective(const array_type &FF) throw()
        {
            static const real_t half(0.5);
            for(size_t i=FF.size();i>0;--i)
                VV[i] = squared(FF[i]);
            return sorted::sum(VV,sorted::by_value) * half;
        }


        template <>
        void zircon<real_t>:: analyze()
        {
            const size_t n = X.size();

            Jt.assign(J,transposed);
            iJ.assign(J);
            Jt(G,F);
            f0 = objective(F);
            std::cerr << "X=" << X << std::endl;
            std::cerr << "F=" << F << std::endl;
            std::cerr << "J=" << J << std::endl;
            std::cerr << "G=" << G << std::endl;
            std::cerr << "f0=" << f0 << std::endl;

            matrix<real_t> J2(n,n);
            vector<real_t> d(n,0);
            matrix<real_t> v(n,n);
            tao::v3::gram(J2,J);
            std::cerr << "J2=" << J2 << std::endl;
            jacobi<real_t> eigen(2);
            if( !eigen(J2,d,v,sort_eigv_by_module) )
            {
                std::cerr << "Bad!" << std::endl;
                return;
            }
            std::cerr << "d=" << d << std::endl;
        }


    }
}
