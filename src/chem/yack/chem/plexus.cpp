
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
        
        M( lib.size() ),
        MA(lib.active() ),
        MP(lib.primary()),
        N( eqs.size() ),
        pre(N),

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
            build_pre();
        }


        static inline
        string build_mixed_name(const int a, const string &A,
                                const int b, const string &B)
        {
            assert(a>0);
            assert(b!=0);
            string ans;
            if(a!=1) ans += vformat("%d*",a);
            ans += A;
            if(b>0)
            {
                ans += '+';
                if(b>1) ans += vformat("%d*",b);
                ans += B;
            }
            else
            {
                assert(b<0);
                if(b<-1) ans += vformat("%d*",b);
                else     ans += '-';
                ans += B;
            }

            return ans;
        }


        equilibrium *build_mixed(int                 a,
                                 const equilibrium  &A,
                                 int                 b,
                                 const equilibrium  &B,
                                 const imatrix      &Nu,
                                 const library      &sub,
                                 const size_t        im)
        {
            assert(a!=0);
            assert(b!=0);
            if(a<0)
            {
                a=-a;
                b=-b;
            }
            const size_t          M    = sub.size(); assert(M==Nu.cols);
            const string          name = build_mixed_name(a,A.name,b,B.name);
            auto_ptr<equilibrium> eptr = new const_equilibrium(name,im,1);
            const readable<int>  &nuA  = Nu[*A];
            const readable<int>  &nuB =  Nu[*B];
            for(const snode *node=sub.head();node;node=node->next)
            {
                const species &s = ***node;
                const size_t   j = *s;
                const int      n = a*nuA[j]+b*nuB[j];
                if(n) (*eptr)(s,n);
            }
            std::cerr << *eptr << std::endl;
            return eptr.yield();
        }

        void plexus:: build_pre()
        {
            YACK_CHEM_PRINTLN("<building_pre>");
            size_t im = 0; // mixed index
            for(const enode *anode=eqs.head();anode;anode=anode->next)
            {
                if(NULL == anode->next) break;
                const equilibrium   &self      = ***anode;
                const size_t         self_indx = *self;
                const readable<int> &self_nu   = Nu[self_indx];
                mixed::list         &self_list = pre[self_indx];

                YACK_CHEM_PRINTLN("\t<" << self.name << ">");
                for(const enode *bnode=anode->next;bnode;bnode=bnode->next)
                {
                    const equilibrium   &peer      = ***bnode;
                    const size_t         peer_indx = *peer;
                    const readable<int> &peer_nu   = Nu[peer_indx];
                    mixed::list         &peer_list = pre[peer_indx];
                    if(verbose)
                    {
                        eqs.pad(std::cerr << "\t /<" << peer.name << ">",peer) << " : " << peer_nu << " (*) " << self_nu << std::endl;
                    }

                    for(size_t j=1;j<=M;++j)
                    {
                        const int A = self_nu[j];
                        const int B = peer_nu[j];
                        if(A!=0 && B!=0)
                        {
                            apz mp_alpha = -B;
                            apz mp_beta  =  A;
                            apz::simplify(mp_alpha,mp_beta);
                            int self_coef = mp_alpha.cast_to<int>("self_coef");
                            int peer_coef = mp_beta.cast_to<int>("peer_coef");
                            assert(self_coef!=0);
                            assert(peer_coef!=0);
                            
                            {
                                const equilibrium::pointer p = build_mixed(self_coef,self,peer_coef,peer,Nu,sub,++im);
                                self_list.push_back( new mixed(self_coef,self_indx,peer_coef,peer_indx,p) );
                            }
                            
                            {
                                const equilibrium::pointer p = build_mixed(peer_coef,peer,self_coef,self,Nu,sub,++im);
                                peer_list.push_back( new mixed(peer_coef,peer_indx,self_coef,self_indx,p) );
                            }
                            
                            
                        }
                    }

                }

                YACK_CHEM_PRINTLN("\t<" << self.name << "/>");

            }
            YACK_CHEM_PRINTLN("<building_pre/>");
            exit(1);
        }


        void plexus:: computeK(const double t)
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                K[*eq] = eq.K(t);
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
