
#include "yack/chem/reactor/solver/steady.hpp"
#include "yack/chem/outcome.hpp"

namespace yack {

    namespace chemical {


        steady:: ~steady() throw() {}

        steady:: steady(const reactor    &_,
                        writable<double> &K_) :
        cs(_),
        K(K_),
        xi(cs.L,0),
        sigma(cs.L,0),
        Corg(cs.M,0),
        Cend(cs.M,0),
        Ctry(cs.M,0),
        Cz(cs.L,cs.L>0?cs.M:0)
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

        void steady:: run(writable<double> &C,
                          const cluster    &cc,
                          const xmlog      &xml)
        {
            YACK_XMLSUB(xml, "steady::cluster" );

            //cc.primary->display(xml,"primary");
            std::cerr << cc.single << std::endl;
            std::cerr << cc.hybrid << std::endl;

            for(const eq_node *en=cc.single->head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = *eq;
                const double       Ki = K[ei];
                writable<double>  &Ci = Cz[ei];
                const outcome      oc = outcome::study(eq,Ki,C,Ci,xmul,xadd);

                cs.all.pad(std::cerr << eq.name,eq) << oc <<  " : ";
                eq.display_compact(std::cerr,Ci);
                std::cerr << std::endl;

                switch(oc.state)
                {
                    case components::are_blocked:

                        break;

                    case components::are_running:

                        break;
                }

            }

        }




    }

}

