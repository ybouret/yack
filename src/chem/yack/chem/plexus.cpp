#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include "yack/arith/gcd.h"
#include "yack/ptr/auto.hpp"

#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        bool plexus::verbose = false;

        plexus:: ~plexus() throw() {}

        const char plexus::clid[] = "chemical::plexus";

        static inline
        size_t check(const library &lib, const equilibria &eqs)
        {
            if(lib.size()<eqs.size())
            {
                throw exception("%s: too many equilibria",plexus::clid);
            }
            return lib.size();
        }

        plexus:: plexus(library &lib_, equilibria &eqs_) :
        lib(lib_),
        eqs(eqs_),
        N(eqs.size()),
        M(check(lib,eqs)),
        A(lib.active()),
        active(),
        conn(N,as_capacity),
        ntab(10,N),
        mtab(10,M),

        K(ntab.next()),
        Gamma(ntab.next()),

        Corg(mtab.next()),
        Ctry(mtab.next()),
        Ctmp(mtab.next()),
        dC(mtab.next()),

        xi(ntab.next()),
        sc(ntab.next()),
        xs(ntab.next()),
        
        Nu(N, (N>0) ? M : 0),
        NuT(Nu.cols,Nu.rows),
        Psi(Nu.rows,Nu.cols),
        W(N,N),
        blocked(N,false),
        
        rstack(M,as_capacity),
        ustack(M,as_capacity),
        LU(N),

        lib_lock( coerce(lib) )
        {
            YACK_CHEM_PRINTLN("#species=" << M );
            YACK_CHEM_PRINTLN("#active =" << A );
            YACK_CHEM_PRINTLN("#eqs    =" << N );

            //------------------------------------------------------------------
            //
            // initialize active
            //
            //------------------------------------------------------------------
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &sp = ***node;
                if(sp.rank>=1)
                {
                    coerce(active) << &sp;
                }
            }


            //------------------------------------------------------------------
            //
            // initialize Nu
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq.validate();
                eq.fill(coerce(Nu[*eq]));
            }

            //------------------------------------------------------------------
            //
            // finalize
            //
            //------------------------------------------------------------------
            coerce(NuT).assign(Nu,transposed);

            if(N>0)
            {
                //--------------------------------------------------------------
                //
                // check equilibria independence
                //
                //--------------------------------------------------------------
                {
                    matrix<apq> G(N,N);
                    tao::v3::gram(G,Nu);
                    YACK_CHEM_PRINTLN( "Nu  = " << Nu );
                    YACK_CHEM_PRINTLN( "NuT = " << NuT);
                    YACK_CHEM_PRINTLN( "G   = " << G  );

                    lu<apq> alu(N);
                    if(!alu.build(G))
                        throw exception("%s: dependant equilibria detected",clid);
                }

                setup_conn();
            }
        }

        static inline void conn_create(int            &a,
                                       int            &b)
        {
            assert(a!=0);
            assert(b!=0);
            uint64_t acoef = absolute(b);
            uint64_t bcoef = absolute(a);
            yack_simplify64(&acoef,&bcoef);

            a = int(acoef);
            b = int(bcoef);
        }


        static inline
        interaction *create_interaction(const int            self_coef,
                                        const string        &self_name,
                                        const int            that_coef,
                                        const string        &that_name,
                                        const readable<int> &self_nu,
                                        const readable<int> &that_nu,
                                        const anode         *node)
        {
            assert(self_coef>0);
            assert(that_coef!=0);
            string name;
            if(self_coef>1) name += vformat("%d*",self_coef);
            name += self_name;
            if(that_coef<0)
            {
                if(that_coef<-1) name += vformat("%d*",that_coef);
                else             name += "-";
            }
            else
            {
                if(that_coef>1) name += vformat("+%d*",that_coef);
                else            name += "+";
            }
            name += that_name;
            interaction          *inter = new interaction(self_coef,that_coef,name);
            auto_ptr<interaction> guard(inter);

            while(node)
            {
                const species &s  = **node;
                const size_t   j  = *s;
                const int      nu = self_coef * self_nu[j] + that_coef * that_nu[j];
                if(nu!=0)
                {
                    inter->add(s,nu);
                }
                node=node->next;
            }
            YACK_CHEM_PRINTLN("//        " << *inter);
            return guard.yield();
        }

        void plexus:: setup_conn()
        {
            //--------------------------------------------------------------
            //
            // building connexions
            //
            //--------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <connexions>");
            for(const enode *lhs=eqs.head();lhs;lhs=lhs->next)
            {

                const equilibrium  &el = ***lhs;
                const size_t        il = *el;
                const imatrix::row &nl = Nu[il];
                connexions *cnx = new connexions(el);
                {
                    const connexions::pointer tmp(cnx);
                    coerce(conn).push_back_fast(tmp);
                }
                YACK_CHEM_PRINTLN("//   connecting " << el.name);
                for(const enode *rhs=eqs.head();rhs;rhs=rhs->next)
                {
                    if(rhs==lhs) continue;
                    const equilibrium  &er = ***rhs;
                    const size_t        ir = *er;
                    const imatrix::row &nr = Nu[ir];
                    //YACK_CHEM_PRINTLN("//       |_with " << er.name);

                    for(const anode *node=active.head;node;node=node->next)
                    {
                        const species  &s  = **node;
                        const size_t    j  = *s;
                        int             a  = nl[j];
                        int             b  = nr[j];
                        const sign_type sa = __sign::of(a);
                        const sign_type sb = __sign::of(b);
                        switch( __sign::pair(sa,sb) )
                        {
                            case zp_pair:
                            case zn_pair:
                            case zz_pair:
                            case pz_pair:
                            case nz_pair:
                                break;

                            case nn_pair:
                            case pp_pair: {
                                conn_create(a,b);
                                b=-b;
                                //std::cerr << "//        " << a << "*" << el.name << " - " << -b << "*" << er.name << std::endl;
                                cnx->sub.push_back( create_interaction(a,el.name,b,er.name,nl,nr,active.head) );
                            } break;

                            case np_pair:
                            case pn_pair: {
                                conn_create(a,b);
                                //std::cerr << "//        " << a << "*" << el.name << " + " << b << "*" << er.name << std::endl;
                                cnx->add.push_back( create_interaction(a,el.name,b,er.name,nl,nr,active.head) );
                            } break;
                        }
                    }

                }
            }
            YACK_CHEM_PRINTLN("// <connexions/>");
        }


        void plexus:: computeK(const double t)
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq(K) = eq.K(t);
            }
        }

        void plexus:: computeGamma(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = *eq;
                Gamma[i] = eq.mass_action(K[i],C);
            }
        }

        void plexus:: computeGammaAndPsi(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = *eq;
                Gamma[i] = eq.drvs_action(Psi[i],K[i],C);
            }
        }


        const readable<double> &plexus:: make_trial(const double u) throw()
        {
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &sp = ***node;
                const size_t   j  = *sp;
                Ctry[j] = max_of<double>(0,Corg[j]+u*dC[j]);
            }
            return Ctry;
        }

        const readable<double> & plexus:: make_trial(const double u, const readable<size_t> &vanishing) throw()
        {
            make_trial(u);
            for(size_t k=vanishing.size();k>0;--k)
            {
                Ctry[ vanishing[k] ] = 0;
            }
            return Ctry;
        }

        void plexus:: save_profile(const char *filename, const double umax)
        {
            ios::ocstream fp(filename);
            YACK_CHEM_PRINTLN("// [saving " << filename << "] -> " << umax);
            for(double u=0;u<1;u+=0.0001)
            {
                const double xx = u * umax;
                fp("%.15g %.15g\n", xx, (*this)(xx) );
            }
        }


        double plexus:: gammaVariance() throw()
        {
            for(size_t i=N;i>0;--i)
            {
                sc[i] = squared(Gamma[i]);
            }

            return sorted::sum(sc,sorted::by_value) / N;
        }

        double plexus:: computeVariance(const readable<double> &C)
        {
            if(N>0)
            {
                computeGamma(C);
                return gammaVariance();
            }
            else
            {
                return 0;
            }

        }

        double plexus:: operator()(const double u)
        {
            make_trial(u);
            return computeVariance(Ctry);
        }


    }
    
}

