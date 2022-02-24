
#include "yack/container/for-each.hpp"
#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/numeric.hpp"
#include "yack/ios/fmt/align.hpp"
#include "yack/math/look-for.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/data/list/sort.hpp"
#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        static inline int compare_inter(const interaction *lhs, const interaction *rhs) throw()
        {
            return comparison::decreasing(lhs->value,rhs->value);
        }


        typedef meta_list<const interaction> iList;
        typedef iList::node_type             iNode;

        static inline int compare_iNode(const iNode *lhs, const iNode *rhs) throw()
        {
            return comparison::decreasing( (**lhs).value, (**rhs).value);
        }


        void plexus:: solve(writable<double> &C)
        {

            switch(N)
            {
                case 0: return;
                case 1: {
                    const equilibrium &eq = ***(eqs.head());
                    eq.solve(K[1],C,Ctmp);
                    computeGammaAndPsi(C);
                } return;

                default:
                    break;
            }
            assert(N>=2);

            //------------------------------------------------------------------
            //
            // fetch initial phase space
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }

            YACK_CHEM_PRINTLN("// <solving>");

            //------------------------------------------------------------------
            //
            // fetch equilibria
            //
            //------------------------------------------------------------------
            vector<equilibrium *> ev(N,NULL);
            vector<double>        kv(N,0);
            vector<size_t>        iv(N,0);

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                ev[ei] = (equilibrium *)&eq;
                kv[ei] = K[ei];
                xs[ei] = eq.extent(kv[ei]=K[ei],Corg,Ctmp);
            }
            // indexing by constants
            indexing::make(iv,comparison::decreasing<double>,kv);
            if(verbose)
            {
                std::cerr << "//   <system>" << std::endl;
                for(size_t k=1;k<=N;++k)
                {
                    const size_t       i  = iv[k];
                    const equilibrium &eq = *ev[i];
                    eqs.pad(std::cerr << "   @" << eq.name,eq);
                    std::cerr << " | K = " << std::setw(14) << kv[i];
                    std::cerr << " | Xs = " << std::setw(14) << xs[i];
                    //std::cerr << " | Nu = " << Nu[i];
                    std::cerr << std::endl;
                }
                std::cerr << "//   <system/>" << std::endl;
            }

            iList IL;
            for(size_t k=1;k<=N;++k)
            {
                const size_t       itop = iv[k];
                const equilibrium &etop = *ev[itop];
                const size_t       row   = *etop;
                size_t             lnk   = 0;

                YACK_CHEM_PRINTLN("//     <" << etop.name << ">");

                for(size_t l=N;l>k;--l)
                {
                    const size_t        isub  = iv[l];
                    const equilibrium  &esub = *ev[isub];
                    const size_t        col  = *esub;
                    const interactions &inter = conn[row][col];
                    //std::cerr << etop.name << " / " << esub.name << " #" << inter.size << std::endl;

                    if(inter.size)
                    {
                        //YACK_CHEM_PRINTLN("//       |_with <" << esub.name << ">");
                        ++lnk;
                        const double Ktop = K[row];
                        const double Ksub = K[col];
                        for(const interaction *in=inter.head;in;in=in->next)
                        {
                            coerce(*in).recompute(Ktop,Ksub);
                        }
                        merge_list_of<interaction>::sort(coerce(inter),compare_inter);
                        const interaction &I = *inter.head;
                        YACK_CHEM_PRINTLN("//       |_" << I);
                        IL << (interaction *) &I;
                    }
                }



                //YACK_CHEM_PRINTLN("//     <" << etop.name << "/>");
            }

            computeGamma(Corg);
            eqs(std::cerr<<"Gamma=",Gamma);

            std::cerr << "Corg=" << Corg << std::endl;
            merge_list_of<iNode>::sort(IL,compare_iNode);
            for(const iNode *node=IL.head;node;node=node->next)
            {
                const interaction &in = **node;
                std::cerr << in << std::endl;
                const double xx = in.extent(*in,Corg,Ctmp);
                dC.ld(0);
                for(const cnode *scan=in.head();scan;scan=scan->next)
                {
                    const component &c = ***scan;
                    const species   &s = *c;
                    const size_t     j = *s;
                    const unit_t     nu = c.nu;
                    dC[j] = xx * nu;
                }
                std::cerr << "dC=" << dC << std::endl;
                triplet<double> X = { 0,0,1 };
                triplet<double> G = { computeVariance(Corg), 0, (*this)(X.c) };
                std::cerr << "G=" << G << std::endl;
                minimize::find<double>::run_for(*this,X,G,minimize::inside);
                std::cerr << "  @X=" << X.b << std::endl;
                tao::v1::set(Corg,Ctry);
                std::cerr << "Corg=" << Corg << std::endl;
            }

            computeGammaAndPsi(Corg);
            eqs(std::cerr<<"Gamma=",Gamma);

            regularize();


            exit(1);



        }

    }

}


