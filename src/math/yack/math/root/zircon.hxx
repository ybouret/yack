
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
        tableaux(8,dims),
        X(  next() ),
        F(  next() ),
        G(  next() ),
        W( next() ),
        S( next()  ),
        XX( next() ),
        VV( next() ),
        f0(0),
        sigma(0),
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
        core::zircon::topology zircon<real_t>:: analyze()
        {

            if(!study.build(U,W,V))
            {
                std::cerr << "cannot SVD..." << std::endl;
                return singular;
            }

            std::cerr << "W=" << W << std::endl;
            const size_t ker = study.ker(W,1e-6);
            std::cerr << "W=" << W << std::endl;
            std::cerr << "ker=" << ker << std::endl;
            if(ker<=0)
            {
                study.solve(U,W,V,S,F);
                sigma = 0;
                for(size_t i=S.size();i>0;--i)
                {
                    XX[i]  = X[i] + tao::v1::neg__(S[i]);
                    sigma -= G[i] * S[i];
                }
                return regular;
            }
            else
            {
                if(ker>=W.size())
                {
                    // bad, bad...
                    return singular;
                }
                else
                {
                    // would use gradient
                    sigma = 0;
                    for(size_t i=S.size();i>0;--i)
                    {
                        sigma += squared( S[i] = -G[i] );
                    }
                    
                    return degenerate;
                }
            }
        }



    }
}
