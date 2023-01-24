
#include "yack/chem/reactor/solver/steady.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/math/iota.hpp"

namespace yack {

    using namespace math;

    namespace chemical {


        steady:: ~steady() throw() {}

        steady:: steady(const reactor    &_,
                        writable<double> &K_) :
        cs(_),
        K(K_),
        xi(cs.L,0),
        sigma(cs.N,0),
        blocked(cs.L,true),
        running(cs.L),
        Corg(cs.M,0),
        Cend(cs.M,0),
        Ctry(cs.M,0),
        Ceq(cs.L,cs.L>0?cs.M:0),
        Phi(cs.N,Ceq.cols),
        Psi(Phi)
        {
        }
        
        void steady:: run(writable<double> &C,
                          const xmlog      &xml)
        {
            for(const cluster *cc = cs.linked->head;cc;cc=cc->next)
            {
                run(C,*cc,xml);
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

        void steady:: run(writable<double> &C,
                          const cluster    &cc,
                          const xmlog      &xml)
        {
            YACK_XMLSUB(xml, "steady::cluster" );

            std::cerr << cc.single << std::endl;
            //std::cerr << cc.hybrid << std::endl;

            YACK_XMLOG(xml,"--> Looking for unsteady...");
            running.clear();
            double             amax = 0;
            const equilibrium *emax = 0;
            for(const eq_node *en=cc.single->head;en;en=en->next)
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
                        Phi[ei].ld(0);
                        Psi[ei].ld(0);
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

            if(amax<=0)
            {
                assert(NULL==emax);
                assert(0   ==running.size);
                YACK_XMLOUT(xml, "--> done");
                return;
            }

            assert(emax!=NULL);
            assert(running.size>0);
            YACK_XMLOG(xml,"--> <" << emax->name << ">");

            if(1==running.size)
            {
                emax->transfer(C,Ceq[**emax]);
                YACK_XMLOUT(xml, "--> done");
                return;
            }

            for(const eq_node *node=running.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                eq.grad_action(Psi[ei], K[ei], Ceq[ei], xmul);
                sigma[ei] = xadd.dot(Psi[ei],cs.Nu[ei]);
                //std::cerr << "sigma_" << eq.name << " = " << sigma[ei] << std::endl;
            }


            assert(running.size>1);
            iota::load(Corg,C);
            double H0 = Hamiltonian(Corg);
            std::cerr << "H0=" << H0 << std::endl;
            for(const eq_node *node=running.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                std::cerr << "H_" << eq.name << " = " << Hamiltonian(Ceq[ei]) << std::endl;
            }
            

        }




    }

}

