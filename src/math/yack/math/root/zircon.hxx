
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
        W(  next() ),
        S(  next()  ),
        XX( next() ),
        FF( next() ),
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
        const zircon<real_t>::array_type & zircon<real_t>:: probe(const real_t u) const throw()
        {
            tao::v1::muladd(XX,X,u,S);
            return XX;
        }


        template <>
        core::zircon::topology zircon<real_t>:: initialize()
        {
            //------------------------------------------------------------------
            //
            // complete local values@X
            //
            //------------------------------------------------------------------
            Jt.assign(J,transposed);
            U.assign(J);
            Jt(G,F);
            f0    = objective(F);
            sigma = 0;

            std::cerr << "X=" << X   << std::endl;
            std::cerr << "F=" << F   << std::endl;
            std::cerr << "J=" << J   << std::endl;
            std::cerr << "G=" << G   << std::endl;
            std::cerr << "f0=" << f0 << std::endl;

            //------------------------------------------------------------------
            //
            // study @X
            //
            //------------------------------------------------------------------
            if(study.build(U,W,V))
            {
                const size_t n = W.size();
                std::cerr << "W   = " << W << std::endl;
                const size_t nullity = study.nullity(W,1e-4);
                std::cerr << "W   = " << W   << std::endl;
                std::cerr << "ker = " << nullity << std::endl;

                if(nullity>=n)
                {
                    // all zero
                    return singular;
                }
                else
                {
                    // evaluate step
                    study.solve(U,W,V,S,F);
                    for(size_t i=n;i>0;--i)
                    {
                        const real_t S_i = tao::v1::neg__(S[i]);
                        XX[i] = X[i] + S_i;
                        VV[i] = S_i * G[i];
                    }
                    std::cerr << "S=" << S << std::endl;
                    sigma = -sorted::sum(VV, sorted::by_abs_value);
                    std::cerr << "sigma=" << sigma << std::endl;

                    if(sigma<=0)
                    {
                        std::cerr << "singular slope" << std::endl;
                        return singular;
                    }

                    if(nullity>0)
                    {
                        return degenerate;
                    }
                    else
                    {
                        return regular;
                    }
                }

            }
            else
            {
                // numeric failure
                return singular;
            }

        }



    }
}
