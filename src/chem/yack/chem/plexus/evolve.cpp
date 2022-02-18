
#include "yack/container/for-each.hpp"
#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/numeric.hpp"
#include "yack/ios/fmt/align.hpp"

#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        void plexus:: evolve( writable<double> &C )
        {

            if(N<=0) return;


            // fetch initial phase space
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }

            if(verbose)
            {
                lib(std::cerr << "C0=", Corg);
            }

            // compute initial state
            computeGammaAndPsi(Corg);

            // regularize all
            if(regularize()>0 && verbose)
            {
                lib(std::cerr << "C0=", Corg);
            }

            YACK_CHEM_PRINTLN("// Gamma = " << Gamma);
            YACK_CHEM_PRINTLN("// Psi   = " << Psi);

            // solve per cluster
            for(const cluster *cls=clusters.head;cls;cls=cls->next)
            {
                solve(*cls);
            }

            // update
            computeGammaAndPsi(Corg);
            tao::v1::set(C,Corg);





        }


        class xpair
        {
        public:
            const equilibrium *eq;
            double             xi;

            inline xpair(const equilibrium *_, const double __) throw() :
            eq(_), xi(__)
            {
            }

            inline ~xpair() throw() {}
            inline xpair(const xpair &other) throw() : eq(other.eq), xi(other.xi)
            {
            }

            static inline int compare(const xpair &lhs, const xpair &rhs) throw()
            {
                return comparison::increasing(lhs.xi,rhs.xi);
            }

        private:
            YACK_DISABLE_ASSIGN(xpair);
        };

        double cluster:: variance(const readable<double> &C, const plexus &sys) const
        {
            vector<double> tmp(size,as_capacity);

            for(const mnode *node=head;node;node=node->next)
            {
                const equilibrium &ei = **node;
                tmp.push_back_fast(squared( ei.mass_action(ei(sys.K),C) ));
            }
            return sorted::sum(tmp,sorted::by_value) / size;
        }


        struct zfn
        {
            plexus        &sys;
            const cluster &cls;

            double operator()(const double u)
            {
                for(size_t j=sys.M;j>0;--j)
                {
                    sys.Ctry[j] = max_of(sys.Corg[j] + u * sys.dC[j],0.0);
                }
                return cls.variance(sys.Ctry,sys);
            }

            double operator()(const double u, const readable<size_t> &vanishing)
            {
                for(size_t j=sys.M;j>0;--j)
                {
                    sys.Ctry[j] = max_of(sys.Corg[j] + u * sys.dC[j],0.0);
                }
                for(size_t k=vanishing.size();k>0;--k)
                {
                    sys.Ctry[ vanishing[k] ] = 0;
                }
                return cls.variance(sys.Ctry,sys);
            }
        };


        void plexus:: solve(const cluster &cls)
        {
            YACK_CHEM_PRINTLN("// <solving cluster#" << cls.indx << ">");
            assert(cls.size>=1);
            switch (cls.size) {
                case 1: {
                    const equilibrium &eq = **cls.head;
                    eq.solve(eq(K),Corg,Ctmp);
                    computeGammaAndPsi(Corg);
                } return;

                default:
                    break;
            }

            const size_t   Nc = cls.size;
            rmatrix        Wc(Nc,Nc);
            vector<double> Xi(Nc,0);

            vector<xpair> state(Nc,as_capacity);
            size_t        iter = 0;

        ITER:
            ++iter;
            YACK_CHEM_PRINTLN("// [iter=" << iter  <<"]");
            state.free();
            for(const mnode *node=cls.head;node;node=node->next)
            {
                const equilibrium &eq = **node;
                const xpair        xp(&eq,eq.extent(eq(K), Corg, Ctmp));
                state.push_back_fast(xp);
            }
            hsort(state,xpair::compare);
            for(size_t i=1;i<=Nc;++i)
            {
                const xpair       &xp = state[i];  assert(xp.eq);
                const equilibrium &eq = *xp.eq;
                eqs.pad(std::cerr << "  @" << eq.name,eq.name) << " = " << xp.xi << std::endl;
            }

            const xpair *head = &state.front();
            const xpair *tail = &state.back();

            if( head->xi * tail->xi < 0 )
            {
                YACK_CHEM_PRINTLN("// reverse directions found");
                double Khead = (*(head->eq))(K);
                double Ktail = (*(tail->eq))(K);

                if(Khead<Ktail)
                {
                    cswap(head,tail);
                    cswap(Khead,Ktail);
                }

                const equilibrium   &eq_head = *(head->eq);
                const readable<int> &nu_head = Nu[*eq_head];
                const equilibrium   &eq_tail = *(tail->eq);
                const readable<int> &nu_tail = Nu[*eq_tail];
                std::cerr << "will solve " << eq_head.name << " (" << Khead <<") and " << eq_tail.name << " (" << Ktail << ")" << std::endl;
                eqs.pad(std::cerr << "Nu_" << eq_head.name,eq_head.name) << " = " << nu_head << std::endl;
                eqs.pad(std::cerr << "Nu_" << eq_tail.name,eq_tail.name) << " = " << nu_tail << std::endl;

                const double      zK =Khead/Ktail;
                const_equilibrium zeq("zeq",zK);
                for(const anode *node=cls.used.head;node;node=node->next)
                {
                    const species &sp = **node;
                    const size_t   j  = *sp;
                    const unit_t   nu = nu_head[j]-nu_tail[j];
                    if(nu!=0)
                    {
                        zeq.add(sp,nu);
                    }
                }
                std::cerr << zeq << std::endl;
                zeq.solve(zK,Corg,Ctmp);
                lib(std::cerr << "Cz=",Corg);
                goto ITER;
                
                throw exception("Not Yet Implemented");
            }
            else
            {
                YACK_CHEM_PRINTLN("// all in the same direction");

                // preparing Wc
                for(const mnode *I=cls.head;I;I=I->next)
                {
                    const equilibrium      &ei  = **I;
                    const size_t            ii  = *ei;
                    const readable<double> &psi = Psi[ii];
                    const size_t            ic  = ei.isub;
                    for(const mnode *J=cls.head;J;J=J->next)
                    {
                        const equilibrium &ej = **J;
                        const size_t       jj = *ej;
                        Wc[ic][ej.isub] = tao::v1::dot<double>::of(psi,Nu[jj]);
                    }
                }
                std::cerr << "Wc=" << Wc << std::endl;

                if( !LU.build(Wc) )
                {
                    throw exception("singular composition");
                }

                // computing Xi
                for(const mnode *I=cls.head;I;I=I->next)
                {
                    const equilibrium      &ei  = **I;
                    const size_t            ii  = *ei;
                    const size_t            ic  = ei.isub;
                    if(blocked[ii])
                    {
                        Xi[ic]     = 0;
                        Wc[ic][ic] = 1;
                    }
                    else
                    {
                        Xi[ic] = -Gamma[ii];
                    }
                }

                LU.solve(Wc,Xi);
                std::cerr << "Xi=" << Xi << std::endl;

                // clamping Xi => xi
                xi.ld(0);
                for(const mnode *I=cls.head;I;I=I->next)
                {
                    const equilibrium      &ei  = **I;
                    const size_t            ii  = *ei;
                    const size_t            ic  = ei.isub;
                    const limits           &lm  = ei.find_primary_limits(Corg);
                    eqs.pad(std::cerr << " @<" << ei.name << ">",ei.name) << " = " << lm << std::endl;
                    xi[ii] = lm.crop(Xi[ic]);
                }
                std::cerr << "xi=" << xi << std::endl;

                // compute dC
                computeDeltaC(cls.excl.head);
                for(const anode *node=cls.excl.head;node;node=node->next)
                {
                    (**node)(dC) = 0;
                }

                double          scale = 1;
                const size_t    count = truncation(scale,cls.used.head);
                triplet<double> x = { 0,0,0 };
                triplet<double> g = { cls.variance(Corg,*this), 0, 0};
                zfn             G = { *this, cls };
                const double    g0 = g.a;
                YACK_CHEM_PRINTLN("// g0=" << g0);
                if(g0<=0)
                {
                    YACK_CHEM_PRINTLN("// [numerical success level-1]");
                    tao::v1::set(Corg,Ctry);
                    return;
                }

                // move
                if(count>0)
                {
                    YACK_CHEM_PRINTLN("// [limited @" << scale << "]");
                    if(scale>1)
                    {
                        YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1]");
                        save_profile("lim.dat",1);
                        g.c = g.b = G(x.c=x.b=1);
                        if(g.c>=g.a)
                        {
                            YACK_CHEM_PRINTLN("// |_[backtrack]");
                            minimize::find<double>::run_for(G,x,g,minimize::inside);
                        }
                        else
                        {
                            YACK_CHEM_PRINTLN("// |_ [accept]");
                        }
                    }
                    else
                    {
                        YACK_CHEM_PRINTLN("// [limited @" << scale << " <= 1]");
                        save_profile("lim.dat",scale);
                        g.c = g.b = G(x.c=x.b=scale,ustack);
                        minimize::find<double>::run_for(G,x,g,minimize::inside);
                        if(g.b>=scale)
                        {
                            // recompute with vanishing
                            g.b = G(x.b=scale,ustack);
                        }
                    }
                }
                else
                {
                    YACK_CHEM_PRINTLN("// [unlimited]");
                    g.c = g.b = G(x.c=x.b=1);
                    save_profile("ulim.dat",x.c);
                    if(g.c>=g.a)
                    {
                        YACK_CHEM_PRINTLN("// |_[backtrack]");
                        minimize::find<double>::run_for(G,x,g,minimize::inside);
                    }
                    else
                    {
                        YACK_CHEM_PRINTLN("// |_ [accept]");
                    }
                }

                // evaluate
                for(const anode *node=cls.used.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    Corg[j] = Ctry[j];
                }

                computeGammaAndPsi(Corg);


                const double g1 = g.b;
                YACK_CHEM_PRINTLN("// g1=" << g1 << " / " << g0);

                if(g1<=0)
                {
                    YACK_CHEM_PRINTLN("// [numerical success level-2]");
                    return;
                }

                if(g1>=g0)
                {
                    YACK_CHEM_PRINTLN("// [numerical convergence]");
                    return;
                }

                goto ITER;

            }

        }


    }

}

