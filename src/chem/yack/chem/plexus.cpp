
#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/ptr/auto.hpp"
#include <cmath>

namespace yack
{
    using  namespace math;

    namespace chemical
    {

        const char   plexus::clid[] = "chemical::plexus";

        const bool & plexus::verbose  = entity::verbose;

        plexus:: ~plexus() throw()
        {
        }

        plexus:: plexus(library    &lib_,
                        equilibria &eqs_) :
        lib( lib_ ),
        eqs( eqs_ ),
        sub( lib  ),
        M( lib.size()    ),
        MA(lib.active()  ),
        MP(lib.primary() ),
        N( eqs.size()    ),
        pre(N),
        NPR(1),
        ntab(10,N),
        mtab(10,M),

        active(),
        K(     ntab.next() ),
        Gamma( ntab.next() ),
        xi(    ntab.next() ),
        xs(    ntab.next() ),
        xm(    ntab.next() ),
        Xi(    ntab.next() ),
        Gs(    ntab.next() ),
        xd(    ntab.next() ),

        Corg(  mtab.next() ),
        Ctmp(  mtab.next() ),
        Ctry(  mtab.next() ),
        Cend(  mtab.next() ),
        dC(    mtab.next() ),

        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),

        NuAT(NuT.rows,NuT.cols),

        Psi(Nu.rows,Nu.cols),
        Ceq(Nu.rows,Nu.cols),
        Omega0(N,N),
        iOmega(N,N),
        blocked(N,false),
        rstack(M,as_capacity),
        ustack(M,as_capacity),
        LU(N),

        lib_lock(lib_),
        eqs_lock(eqs_)
        {

            YACK_CHEM_PRINTLN("// M  = " << M);
            YACK_CHEM_PRINTLN("// MA = " << MA);
            YACK_CHEM_PRINTLN("// MP = " << MP);

            YACK_CHEM_PRINTLN("// N  = " << N);

            //------------------------------------------------------------------
            //
            // build active species
            //
            //------------------------------------------------------------------
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                if(s.rank>0)
                {
                    coerce(active) << &s;
                }
            }
            assert(MA==active.size);

            //------------------------------------------------------------------
            //
            // build Nu
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                if(!eq.is_minimal()) throw exception( "%s is not minimal", eq.name() );
                if(!eq.is_neutral()) throw exception( "%s is not neutral", eq.name() );
                eq.fill( coerce(Nu[*eq]) );
            }
            coerce(NuT).assign(Nu,transposed);
            YACK_CHEM_PRINTLN("Nu =" << Nu);
            YACK_CHEM_PRINTLN("NuT=" << NuT);

            //------------------------------------------------------------------
            //
            // check indep equilibria
            //
            //------------------------------------------------------------------
            if(N>0)
            {
                matrix<apq> G(N,N);
                lu<apq>     alu(N);
                tao::v3::gram(G,Nu);
                if( !alu.build(G) ) throw exception("%s: dependent equilibria",clid);
            }

            //------------------------------------------------------------------
            //
            // build pre
            //
            //------------------------------------------------------------------
            coerce(NPR) += build_pre();
        }




        size_t plexus:: build_pre()
        {
            YACK_CHEM_PRINTLN("<building_pre>");
            size_t im=0;
            size_t nrmax=0;

            for(const enode *anode=eqs.head();anode;anode=anode->next)
            {
                const equilibrium   &eqA = ***anode;
                const readable<int> &nuA = Nu[*eqA];
                mixed::list         &mxa = pre[*eqA];
                YACK_CHEM_PRINTLN("  <" << eqA.name << ">");
                for(const enode *bnode=eqs.head();bnode;bnode=bnode->next)
                {
                    if(bnode==anode) continue;
                    const equilibrium   &eqB = ***bnode;
                    const readable<int> &nuB = Nu[*eqB];
                    for(size_t j=1;j<=M;++j)
                    {
                        const int A = nuA[j];
                        const int B = nuB[j];
                        if(A!=0 && B!=0)
                        {
                            apz mp_alpha = -B;
                            apz mp_beta  =  A;
                            apz::simplify(mp_alpha,mp_beta);

                            int alpha = mp_alpha.cast_to<int>("alpha");
                            int beta  = mp_beta.cast_to<int>("beta");
                            if(alpha<0)
                            {
                                alpha = -alpha;
                                beta  = -beta;
                            }
                            //std::cerr << "\talpha = " << alpha << std::endl;
                            //std::cerr << "\tbeta  = " << beta  << std::endl;
                            const string name = vformat("(%d)*%s+(%d)*%s",alpha,eqA.name(),beta,eqB.name());
                            mixed *mx = mxa.push_back( new mixed(name,++im,alpha,K[*eqA],beta,K[*eqB]));

                            for(size_t k=1;k<=M;++k)
                            {
                                const int nu = alpha * nuA[k] + beta * nuB[k];
                                if(nu!=0)
                                {
                                    (*mx)( sub[k], nu );
                                }
                            }


                            std::cerr << "    " << *mx << std::endl;
                        }
                    }
                }
                nrmax = max_of(nrmax,mxa.size);
                YACK_CHEM_PRINTLN("  <" << eqA.name << "/>");
            }

            YACK_CHEM_PRINTLN("  nrmax=" << nrmax);
            YACK_CHEM_PRINTLN("<building_pre/>");
            return nrmax;
        }


        void plexus:: computeK(const double t)
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                K[*eq] = eq.K(t);
            }

            for(size_t i=N;i>0;--i)
            {
                for(mixed *mx=pre[i].head;mx;mx=mx->next)
                {
                    mx->updateK();
                }
            }
        }

        void plexus:: computeGamma(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Gamma[ei] = eq.mass_action(K[ei],C);
            }
        }

        void plexus:: computePsi(const readable<double> &C) throw()
        {
            Psi.ld(0);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                eq.drvs_action(Psi[ei],K[ei],C,Ctmp);
            }
        }

        void plexus:: computeState(const readable<double> &C) throw()
        {
            Psi.ld(0);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       Ki = K[ei];
                Gamma[ei] = eq.grad_action(Psi[ei],Ki,C,Ctmp);
            }
        }



        void plexus:: transfer(writable<double> &targetC, const readable<double> &sourceC) const throw()
        {
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                targetC[j] = sourceC[j];
            }
        }

        bool plexus:: are_valid(const readable<double>  &C) throw()
        {
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                if(C[j]<0) return false;
            }
            return true;
        }

        
        
    }

}
