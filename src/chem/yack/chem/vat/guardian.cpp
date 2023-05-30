
#include "yack/chem/vat/guardian.hpp"

namespace yack
{
    namespace Chemical
    {
        Guardian:: ~Guardian() noexcept {}

        Guardian:: Guardian() :
        broken(),
        excess(),
        xadd()
        {
        }

        void Guardian:: restart(const size_t M)
        {
            const XReal zero(0);
            adjust(M,zero);
            ld(zero);
        }

        void Guardian:: enforce(const xmlog      &xml,
                                writable<double> &C0,
                                const       Act  &act)
        {
            YACK_XMLSUB(xml,"Guardian::enforce");
            writable<XReal> &Me = *this;
            //------------------------------------------------------------------
            //
            //
            // initialize broken laws
            //
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, " (*) initializing");
            broken.clear();
            excess.clear();
            for(const LawNode *node=act.head;node;node=node->next)
            {
                const ConservationLaw &law = ***node;
                const XReal            lxs = law.excess(C0,xadd,TopLevel);
                assert(lxs>=0);

                bool ok = true;
                if(lxs.m>0)
                {
                    ok = false;
                    broken << law;
                    excess << lxs;
                }
                if(xml.verbose)
                    act.pad(*xml << (ok ? " (-) " : " (+) ") << law, law) << " = " << lxs << std::endl;
            }

            //------------------------------------------------------------------
            //
            //
            // process
            //
            //
            //------------------------------------------------------------------
            while(broken.size)
            {
                assert(broken.size == excess.size);

                //--------------------------------------------------------------
                //
                // find best
                //
                //--------------------------------------------------------------
                LawNode *bestBroken = broken.head;
                XsNode  *bestExcess = excess.head;

                {
                    LawNode *nodeBroken = bestBroken;
                    XsNode  *nodeExcess = bestExcess;
                    for(nodeBroken=nodeBroken->next,nodeExcess=nodeExcess->next;
                        nodeBroken;
                        nodeBroken=nodeBroken->next,nodeExcess=nodeExcess->next)
                    {
                        if( **nodeExcess< **bestExcess)
                        {
                            bestExcess = nodeExcess;
                            bestBroken = nodeBroken;
                        }
                    }
                }

                //--------------------------------------------------------------
                //
                // update values
                //
                //--------------------------------------------------------------
                {
                    const ConservationLaw &law = ***bestBroken;
                    const XReal            lxs = **bestExcess;
                    if(xml.verbose)
                        act.pad(*xml << " --> " << law, law) << " = " << lxs << " <--" << std::endl;

                    const XReal      d = law.Q2;
                    const Actor     *a = law.head;    assert(a); assert(law.size==law.Q.size);
                    const CoreXNode *q = law.Q.head;  assert(q);
                    for(;a;a=a->next,q=q->next)
                    {
                        const XReal  dC = (lxs * (**q))/d;
                        const size_t sj = (**a).indx[TopLevel];
                        Me[sj] += dC;
                        C0[sj] += *dC;
                    }
                }

                //--------------------------------------------------------------
                //
                // remove selected from list
                //
                //--------------------------------------------------------------
                broken.cut(bestBroken);
                excess.cut(bestExcess);

                //--------------------------------------------------------------
                //
                // update remaining
                //
                //--------------------------------------------------------------
                {
                    LawRepo          tempBroken;
                    core_list<XReal> tempExcess;
                    while(broken.size)
                    {
                        const XReal lxs = (***broken.head).excess(C0,xadd,TopLevel);
                        if(lxs>0)
                        {
                            (void) tempBroken.push_back( broken.pop_front() );
                            **     tempExcess.push_back( excess.pop_front() ) = lxs;
                        }
                        else
                        {
                            broken.cut_head();
                            excess.cut_head();
                        }
                    }
                    broken.swap_with(tempBroken);
                    excess.swap_with(tempExcess);
                }

            }

        }

        void Guardian:: operator()(const xmlog            &xml,
                                   writable<double>       &C0,
                                   const list_of<Cluster> &clusters)
        {
            YACK_XMLSUB(xml,"Gardian::enforceAll");
            restart(C0.size());
            for(const Cluster *cluster = clusters.head; cluster; cluster=cluster->next)
            {
                for(const Act *act = cluster->acts.head; act; act=act->next)
                {
                    enforce(xml,C0,*act);
                }
            }

        }

    }

}
