
#include "yack/chem/vat/custodian.hpp"

namespace yack
{
    namespace Chemical
    {
        Custodian:: ~Custodian() noexcept {}

        Custodian:: Custodian(const size_t maximumSpecies) :
        Injected(maximumSpecies,as_capacity),
        broken(),
        excess(),
        xadd()
        {
        }

        void Custodian:: restart(const size_t M)
        {
            Injected &self = *this;
            while( self.size() < M )
            {
                const Extended::AddPtr ptr = new Extended::Add();
                push_back(ptr);
            }
            assert(self.size()>=M);
            for(size_t i=self.size();i>0;--i) self[i]->free();

        }

        void Custodian:: enforce(const xmlog              &xml,
                                writable<Extended::Real> &C0,
                                const       Act          &act)
        {
            YACK_XMLSUB(xml,"Guardian::enforceAct");
            writable<Extended::AddPtr> &Me = *this; assert(Me.size()>=C0.size());
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
                const Extended::Real   lxs = law.excess(C0,xadd,TopLevel);
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
                    const Extended::Real   lxs =  **bestExcess;
                    if(xml.verbose)
                        act.pad(*xml << " --> " << law, law) << " = " << lxs << " <--" << std::endl;

                    const Extended::Real  d = law.Q2;
                    const Actor          *a = law.head;    assert(a);
                    for(;a;a=a->next)
                    {
                        const Extended::Real dC = (lxs * (a->xn))/d;
                        const size_t         sj = (**a).indx[TopLevel];
                        Me[sj]->append(dC);
                        C0[sj] += dC;
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
                    LawRepo            tempBroken;
                    Extended::CoreList tempExcess;
                    while(broken.size)
                    {
                        const Extended::Real lxs = (***broken.head).excess(C0,xadd,TopLevel);
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

        void Custodian:: enforce(const xmlog              &xml,
                                writable<Extended::Real> &Corg,
                                const Cluster            &cluster)
        {
            YACK_XMLSUB(xml,"Guardian::enforce");
            for(const Act *act = cluster.acts.head; act; act=act->next)
                enforce(xml,Corg,*act);
        }

    }

}
