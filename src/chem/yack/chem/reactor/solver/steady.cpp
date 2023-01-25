
#include "yack/chem/reactor/solver/steady.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/temporary.hpp"
#include <iomanip>

namespace yack {

    using namespace math;

    namespace chemical {


        bool er_repo:: ratifies(const squad &sq) const throw()
        {
            for(const squad::node_type *node=sq.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                if( !contains(eq) ) return false;
            }
            return true;
        }



        std::ostream & operator<<(std::ostream &os, const er_repo &self)
        {
            os << "{ ";
            const eq_node *node = self.head;
            if(node)
            {
                os << (***node).name;
                for(node=node->next;node;node=node->next)
                {
                    os << ", " << (***node).name;
                }
            }
            os << " }";
            return os;
        }


        double steady:: Hamiltonian(writable<double> &Cout, const squad &sq)
        {

            // initialize
            iota::load(Cout,Corg);

            // transfer components
            for(const squad::node_type *node=sq.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq.transfer(Cout,Ceq[*eq]);
            }

            return Hamiltonian(Cout);

        }

        steady:: ~steady() throw() {}

        steady:: steady(const reactor    &_,
                        writable<double> &K_) :
        cs(_),
        cc(NULL),
        K(K_),
        xi(cs.L,0),
        sigma(cs.N,0),
        blocked(cs.L,true),
        running(cs.N),
        solving(cs.L),
        Corg(cs.M,0),
        Cend(cs.M,0),
        Ctry(cs.M,0),
        Ceq(cs.L,cs.L>0?cs.M:0),
        Phi(cs.N,Ceq.cols),
        Psi(Phi),
        xmul(),
        xadd()
        {
        }
        
        void steady:: run(writable<double> &C,
                          const xmlog      &xml)
        {
            for(const cluster *cls = cs.linked->head;cls;cls=cls->next)
            {
                run(C,*cls,xml);
            }
        }


        double steady:: Hamiltonian(const readable<double> &C)
        {
            xadd.ldz();
            for(const eq_node *node=running.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;     assert( fabs(sigma[ei])>0 );
                xadd.push( squared( eq.mass_action(K[ei],C,xmul)/sigma[ei] ) );
            }
            return xadd.get()/2;
        }

        double steady:: operator()(const double u)
        {
            assert(NULL!=cc);
            iota::load(Ctry,Corg);

            for(const anode *an = (*(cc->alive))->head; an; an=an->next)
            {
                const species &s = an->host;
                const size_t   j = *s;
                if(u<=0)
                {
                    Ctry[j] = Corg[j];
                }
                else
                {
                    if(u>=1)
                    {
                        Ctry[j] = Cend[j];
                    }
                    else
                    {
                        const double c0   = Corg[j];
                        const double c1   = Cend[j];
                        double       cmin = c0;
                        double       cmax = c1;
                        if(cmax<cmin)
                            cswap(cmin,cmax);
                        assert(cmin<=cmax);
                        Ctry[j] = clamp(cmin,c0*(1.0-u)+u*c1,cmax);
                    }
                }
            }

            return Hamiltonian(Ctry);
        }

        double steady:: optimized_H(const double H0)
        {
            triplet<double> x = {  0, -1,                1  };
            triplet<double> f = { H0, -1, Hamiltonian(Cend) };
            steady         &F = *this;

            assert( fabs( Hamiltonian(Corg) - f.a) <= 0);
            assert( fabs( Hamiltonian(Cend) - f.c) <= 0);

            optimize::run_for(F, x, f, optimize::inside);

            assert( fabs(f.b - F(x.b)) <= 0 );

            return f.b;
        }


        void steady:: run(writable<double> &C,
                          const cluster    &cls,
                          const xmlog      &xml)
        {
            YACK_XMLSUB(xml, "steady::cluster" );               assert(NULL==cc);
            const temporary<const cluster*> momentary(cc,&cls); assert(NULL!=cc);

            std::cerr << cc->single << std::endl;

            YACK_XMLOG(xml,"--> Looking for most unsteady");

            size_t cycle = 0;
        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // let's inspect the system and initialize
            //
            //------------------------------------------------------------------
            running.clear();
            double             amax = 0;
            const equilibrium *emax = 0;
            for(const eq_node *en=cc->single->head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = *eq;
                const double       Ki = K[ei];
                writable<double>  &Ci = Ceq[ei];
                const outcome      oc = outcome::study(eq,Ki,C,Ci,xmul,xadd);

                if(xml.verbose)
                {
                    cs.all.pad(*xml << eq.name,eq) << oc <<  " : ";
                    eq.display_compact(xml(),Ci);
                    xml() << std::endl;
                }

                switch(oc.state)
                {
                    case components::are_blocked:
                        blocked[ei] = true;
                        xi[ei]      = 0;
                        sigma[ei]   = 0;
                        break;

                    case components::are_running: {
                        blocked[ei] = false;
                        running    << eq;
                        const double atmp = fabs(xi[ei]=oc.value);
                        if(atmp>amax)
                        {
                            amax = atmp;
                            emax = &eq;
                        }
                    } break;
                }
            }


            //------------------------------------------------------------------
            //
            // all good within numerical limit!
            //
            //------------------------------------------------------------------
            if(amax<=0)
            {
                assert(NULL==emax);
                assert(0   ==running.size);
                YACK_XMLOUT(xml, "--> done");
                return;
            }

            //------------------------------------------------------------------
            //
            // process
            //
            //------------------------------------------------------------------
            assert(emax!=NULL);
            assert(running.size>0);
            YACK_XMLOG(xml,"--> <" << emax->name << "> <--");

            //------------------------------------------------------------------
            //
            // trivial case
            //
            //------------------------------------------------------------------
            if(1==running.size)
            {
                emax->transfer(C,Ceq[**emax]);
                YACK_XMLOUT(xml, "--> done");
                return;
            }


            //------------------------------------------------------------------
            //
            // compute scalings
            //
            //------------------------------------------------------------------
            for(const eq_node *node=running.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;

                eq.grad_action(Psi[ei], K[ei], Ceq[ei], xmul);
                sigma[ei] = xadd.dot(Psi[ei],cs.Nu[ei]); assert(sigma[ei] < 0);
                if(xml.verbose) cs.all.pad(*xml << "sigma_<" << eq.name << ">", eq) << " = " << std::setw(15) << sigma[ei] << std::endl;
            }


            //------------------------------------------------------------------
            //
            // initialize search
            //
            //------------------------------------------------------------------
            assert(running.size>1);
            iota::load(Corg,C);
            double H0 = Hamiltonian(Corg);
            YACK_XMLOG(xml, "H0 = " << H0);

            //------------------------------------------------------------------
            //
            // search running singles
            //
            //------------------------------------------------------------------
            solving.clear();
            for(const eq_node *node=running.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Ceq[ei];         iota::load(Cend,Ci);
                const double       H1 = optimized_H(H0); iota::load(Ci,Ctry);
                const bool         ok = (H1<H0);
                assert( fabs(H1-Hamiltonian(Ctry)) <=0 );
                if(xml.verbose) { cs.all.pad(*xml << (ok? "[+]" : "[-]") << " H_<" << eq.name << ">",eq) << " = " << std::setw(15) << H1 << std::endl; }
                if(ok)
                {
                    solving << eq;
                }
            }


            //------------------------------------------------------------------
            //
            // search hybrid
            //
            //------------------------------------------------------------------
            for(const eq_node *node=cc->hybrid->head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const outcome      oc = outcome::study(eq, K[ei], Corg, Cend, xmul, xadd);
                if(false && xml.verbose)
                {
                    cs.all.pad(*xml << eq.name,eq) << oc <<  " : ";
                    eq.display_compact(xml(),Cend);
                    xml() << std::endl;
                }
                switch(oc.state)
                {
                    case components::are_blocked:
                        blocked[ei] = true;
                        xi[ei]      = 0;
                        break;

                    case components::are_running: {
                        // Cend is loaded
                        const double H1 = optimized_H(H0); assert( fabs(H1-Hamiltonian(Ctry)) <=0 );
                        const bool   ok = (H1<H0);
                        if(xml.verbose) { cs.all.pad(*xml << (ok? "[+]" : "[-]") << " H_<" << eq.name << ">",eq) << " = " << std::setw(15) << H1 << std::endl; }
                        if(ok)
                        {
                            solving << eq;
                            iota::load(Ceq[ei],Ctry);
                        }
                    } break;
                }
            }


            std::cerr << "solving=" << solving << std::endl;

            if(solving.size<=0)
            {
                std::cerr << "No Global Min @cycle=" << cycle << std::endl;
                exit(0);
            }

            const squad *best = NULL;
            double       Hopt = -1;

            // initialize best and Hopt
            for(const squad *sq = cc->army->head; sq; sq=sq->next )
            {
                if(!solving.ratifies(*sq)) continue;
                best = sq;
                Hopt = Hamiltonian(Cend,*best);
                YACK_XMLOG(xml, std::setw(15) << Hopt << " @ " << *best);
                break;
            }

            assert(NULL!=best);
            assert(Hopt>=0);

            // look for better
            for(const squad *sq=best->next;sq;sq=sq->next)
            {
                if(!solving.ratifies(*sq)) continue;
                const double temp = Hamiltonian(Ctry,*sq);
                if(temp<Hopt) {
                    best = sq;
                    Hopt = temp;
                    iota::load(Cend,Ctry);
                    YACK_XMLOG(xml, std::setw(15) << Hopt << " @ " << *best);
                }
            }

            iota::save(C,Cend);
            std::cerr << "@cycle " << cycle << std::endl;
            goto CYCLE;


        }




    }

}

