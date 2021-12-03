
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
        tableaux(6,dims),
        X(  next() ),
        F(  next() ),
        G(  next() ),
        mu( next() ),
        S( next()  ),
        VV( next() ),
        f0(0),
        J(dims,dims),
        Jt(dims,dims),
        J2(dims,dims),
        V(dims,dims),
        Vt(dims,dims),
        eigen(dims),
        fdjac(0)
        {
        }

        template <>
        void zircon<real_t>:: setup(size_t dims)
        {
            make(dims);
            J.make(dims,dims);
            Jt.make(dims,dims);
            J2.make(dims,dims);
            V.make(dims,dims),
            Vt.make(dims,dims);
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
        void zircon<real_t>:: initialize()
        {
            //const size_t n = X.size();

            Jt.assign(J,transposed);
            Jt(G,F);
            f0 = objective(F);
            std::cerr << "X=" << X   << std::endl;
            std::cerr << "F=" << F   << std::endl;
            std::cerr << "J=" << J   << std::endl;
            std::cerr << "G=" << G   << std::endl;
            std::cerr << "f0=" << f0 << std::endl;
            tao::v3::gram(J2,Jt);
            std::cerr << "J2=" << J2 << std::endl;
        }

        template <>
        bool zircon<real_t>:: analyze()
        {
            static const real_t ftol = numeric<real_t>::ftol;
            const size_t n = mutual_size();

            if( !eigen(J2,mu,V,sort_eigv_by_module))
            {
                std::cerr << "no eigen value" << std::endl;
                return false;
            }

            std::cerr << "mu=" << mu << std::endl;
            const real_t threshold = fabs(mu[1]) * ftol;
            size_t       ker = 0;
            size_t       img = n;
            while(ker<n&& fabs(mu[img])<=threshold)
            {
                mu[img] = 0;
                ++ker;
                --img;
            }
            std::cerr << "img=" << img << std::endl;
            std::cerr << "ker=" << ker << std::endl;
            std::cerr << "mu =" << mu  << std::endl;



            return true;

        }

    }
}
