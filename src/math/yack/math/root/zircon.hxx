
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
        W( next() ),
        S( next()  ),
        VV( next() ),
        f0(0),
        J(dims,dims),
        Jt(dims,dims),
        U(dims,dims),
        V(dims,dims),
        study(dims),
        fdjac(0)
        {
        }

        template <>
        void zircon<real_t>:: setup(size_t dims)
        {
            make(dims);
            J.make(dims,dims);
            Jt.make(dims,dims);
            U.make(dims,dims);
            V.make(dims,dims);
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

            Jt.assign(J,transposed);
            U.assign(J);
            Jt(G,F);
            f0 = objective(F);
            
            std::cerr << "X=" << X   << std::endl;
            std::cerr << "F=" << F   << std::endl;
            std::cerr << "J=" << J   << std::endl;
            std::cerr << "G=" << G   << std::endl;
            std::cerr << "f0=" << f0 << std::endl;

        }

        template <>
        bool zircon<real_t>:: analyze()
        {
            //const size_t n = mutual_size();

            if(!study.build(U,W,V))
            {
                std::cerr << "cannot SVD..." << std::endl;
                return false;
            }

            std::cerr << "W=" << W << std::endl;
            const size_t ker = study.ker(W,1e-6);
            std::cerr << "W=" << W << std::endl;
            std::cerr << "ker=" << ker << std::endl;





            return true;

        }

    }
}
