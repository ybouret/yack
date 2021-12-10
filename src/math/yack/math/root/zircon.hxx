
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
        real_t zircon<real_t>:: control(const array_type &FF) throw()
        {
            static const real_t half(0.5);
            for(size_t i=FF.size();i>0;--i)
                VV[i] = squared(FF[i]);
            return sorted::sum(VV,sorted::by_value) * half;
        }

        template <>
        const zircon<real_t>::array_type & zircon<real_t>:: probe(const real_t u) throw()
        {
            tao::v1::muladd(XX,X,u,S);
            return XX;
        }

        template <>
        core::zircon::topology zircon<real_t>:: lookup(f1d &f1)
        {
            //------------------------------------------------------------------
            //
            // complete local values@X
            //
            //------------------------------------------------------------------
            static const char fn[] = "[zircon] ";
            YACK_ZIRCON_PRINTLN(fn << "lookup");
            
            Jt.assign(J,transposed); // Jt = J'
            U.assign(J);             // U  = J
            Jt(G,F);                 // G = J'*F, gradient of control

            triplet<real_t> f = { control(F), -1, -1 };
            triplet<real_t> u = { 0,          -1,  1 };
            YACK_ZIRCON_PRINTLN("X     = " << X);
            YACK_ZIRCON_PRINTLN("F     = " << F);
            YACK_ZIRCON_PRINTLN("J     = " << J);
            YACK_ZIRCON_PRINTLN("G     = " << G);
            YACK_ZIRCON_PRINTLN("f0    = " << f.a);

            //------------------------------------------------------------------
            //
            // decompose jacobian
            //
            //------------------------------------------------------------------
            if(!study.build(U,W,V))
            {
                //! bad matrix!
                YACK_ZIRCON_PRINTLN(fn<<"invalid jacobian");
                return singular;
            }

            //------------------------------------------------------------------
            //
            // check nullity
            //
            //------------------------------------------------------------------
            const size_t dimension = W.size();                     YACK_ZIRCON_PRINTLN("W     = " << W);
            const size_t nullity   = svd<real_t>::nullity(W,1e-4);

            if(nullity>0)
            {
                YACK_ZIRCON_PRINTLN("W     = " << W);
                YACK_ZIRCON_PRINTLN("nil   = " << nullity);
            }

            if(nullity>=dimension)
            {
                YACK_ZIRCON_PRINTLN(fn<<"nul jacobian");
                return singular;
            }

            //------------------------------------------------------------------
            //
            // construct step
            //
            //------------------------------------------------------------------
            study.solve(U,W,V,S,F);
            for(size_t i=dimension;i>0;--i)
            {
                const real_t S_i = tao::v1::neg__(S[i]);
                //XX[i] = X[i] + S_i;
                VV[i] = S_i * G[i];
            }

            //------------------------------------------------------------------
            //
            // evaluate decrease rate of the control function
            //
            //------------------------------------------------------------------
            real_t sigma = -sorted::sum(VV, sorted::by_abs_value);
            YACK_ZIRCON_PRINTLN("S     = " << S);
            YACK_ZIRCON_PRINTLN("sigma = " << sigma);
            if(sigma<=0)
            {
                YACK_ZIRCON_PRINTLN(fn<<"zero numerical slope");
                return singular;
            }

            //------------------------------------------------------------------
            //
            // prepare triplet using initial full step
            //
            //------------------------------------------------------------------
            f.c = f1(u.c);
            real_t slope = 0.1*sigma;
            if(f.c>f.a-slope*u.c)
            {
                //--------------------------------------------------------------
                //
                // backtrack using reduced intervals and
                // deducing optimal point by cubic interpolation
                //
                //--------------------------------------------------------------
            BACKTRACK:
                YACK_ZIRCON_PRINTLN(fn<<"<bactrack>");
                f.b = f1(u.b=u.c/2);
                YACK_ZIRCON_PRINTLN("\tu=" << u << ", f=" << f);
                while(f.b>f.a-slope*u.b)
                {
                    f.c = f.b;
                    u.c = u.b;
                    f.b = f1(u.b/=2);
                }
                YACK_ZIRCON_PRINTLN("\tu=" << u << ", f=" << f);
                const real_t delta_b = f.b - f.a;
                const real_t delta_c = f.c - f.a;
                const real_t sigma_c = sigma*u.c;
                const real_t beta    = delta_c - 8 * delta_b - 3*sigma_c;
                const real_t gamma   = twice(delta_c - 4*delta_b - sigma_c);
                YACK_ZIRCON_PRINTLN("\tbeta = " << beta << "; gamma=" << gamma);
                if(beta<=0||gamma<=0)
                {
                    YACK_ZIRCON_PRINTLN("spurious singular backtracking point");
                    return singular;
                }
                {
                    ios::ocstream fp("zircon.dat");
                    for(real_t uu=0;uu<=u.c;uu+=(0.001*u.c))
                    {
                        const real_t ff = f1(uu);
                        fp("%.15g %.15g %.15g\n", double(uu), double(ff), double(f.a-sigma*uu-beta*squared(uu/u.c)+gamma*cubed(uu/u.c)) );
                    }
                }
                const real_t three_gamma = 3*gamma;
                u.c *= (beta+sqrt(beta*beta+sigma_c*three_gamma))/three_gamma;
                f.c  = f1(u.c);
                YACK_ZIRCON_PRINTLN("\tf(" << u.c << ")=" << f.c);
                if(f.c>f.a-slope*u.c)
                    goto BACKTRACK;

            }
            else
            {
                //--------------------------------------------------------------
                //
                // accept full step
                //
                //--------------------------------------------------------------
                YACK_ZIRCON_PRINTLN(fn<<"<full step>");
                ios::ocstream fp("zircon.dat");
                for(real_t uu=0;uu<=u.c;uu+=(0.001*u.c))
                {
                    const real_t ff = f1(uu);
                    fp("%.15g %.15g %.15g\n", double(uu), double(ff), double(f.a-sigma*uu) );
                }
            }

            YACK_ZIRCON_PRINTLN(fn<<"<new position>");
            std::cerr << "XX=" << XX << std::endl;



            return (nullity<=0) ? regular : degenerate;
        }



    }
}
