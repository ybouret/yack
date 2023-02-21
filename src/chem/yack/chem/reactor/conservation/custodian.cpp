
#include "yack/chem/reactor/conservation/custodian.hpp"
#include "yack/math/iota.hpp"
#include "yack/type/boolean.h"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        custodian:: custodian(const reactor &_) :
        cs(_),
        nc( cs.max_claws_per_cluster() ),
        xs(nc),
        sr(nc),
        Cs(nc,nc>0?cs.M:0),
        dC(cs.M),
        io(nc)
        {
            std::cerr << "custodian: nc=" << nc << std::endl;
        }

        custodian:: ~custodian() noexcept
        {
            
        }

        void custodian:: reset_all() noexcept
        {
            sr.clear();
            xs.ld(0);
            dC.ld(0);
            Cs.ld(0);
        }

        bool custodian:: corrected(writable<double> &C0)
        {
            const xmlog xml("[reactor.custodian]", std::cerr, reactor::verbose);
            YACK_XMLSUB(xml,"correcting");
            assert(C0.size()>=cs.M);
            dC.ld(0);
            bool did = false;
            for(const cluster *cc=cs.linked->head;cc;cc=cc->next)
            {
                YACK_XMLSUB(xml, "cluster");
                for(const clot *cg=cc->claim->head;cg;cg=cg->next)
                {
                    if( corrected(C0,*cg,xml) ) {
                        did = true;
                    }
                }
            }
            return did;
        }


        bool custodian:: corrected(writable<double> &C0,
                                   const clot       &cg,
                                   const xmlog      &xml)
        {
            assert(cg.size<=nc);

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            sr.clear();
            for(const cl_node *node=cg.head;node;node=node->next)
            {
                const claw  &cl = ***node;
                const size_t ic = *cl;
                if( cl.excess(C0, Cs[ic], io) )
                {
                    sr    << cl;
                    xs[ic] = io.get();
                    YACK_XMLOG(xml," (+) " << std::setw(15) << xs[ic] << " @" << cl);
                }
            }

            //------------------------------------------------------------------
            //
            // check no excess
            //
            //------------------------------------------------------------------
            if(sr.size<=0) return false;

            // temporary stack for algorithm
            cl_repo cstk;

            //------------------------------------------------------------------
            //
            // cycling over corrections
            //
            //------------------------------------------------------------------
            while(true)
            {
                assert(sr.size>0);

                //--------------------------------------------------------------
                //
                // find and apply minimal correction
                //
                //--------------------------------------------------------------
                {
                    //----------------------------------------------------------
                    // initialize search
                    //----------------------------------------------------------
                    cl_node *best = sr.head;
                    double   tiny = xs[ ****best ];

                    //----------------------------------------------------------
                    // find smaller correction
                    //----------------------------------------------------------
                    for(cl_node *node=best->next;node;node=node->next)
                    {
                        const double temp = xs[****node];
                        if(temp<tiny) {
                            best = node;
                            tiny = temp;
                        }
                    }

                    YACK_XMLOG(xml," (*) " << std::setw(15) << tiny << " @" << *best);

                    //----------------------------------------------------------
                    // query winning
                    //----------------------------------------------------------
                    const claw             &cwin = ***best;
                    const size_t            iwin = *cwin;
                    const readable<double> &Cwin = Cs[iwin];

                    //----------------------------------------------------------
                    // update dC
                    //----------------------------------------------------------
                    for(const actor *a = cwin->head;a;a=a->next)
                    {
                        const size_t j = ***a;
                        dC[j] += Cwin[j] - C0[j];
                    }
                    YACK_XMLOG(xml,"--> dC=" << dC);
                    //----------------------------------------------------------
                    // update C0 and remove best
                    //----------------------------------------------------------
                    iota::save(C0,Cwin);
                    sr.cut(best);
                }

                if(sr.size<=0) return true;

                //--------------------------------------------------------------
                //
                // recompute status without former best
                //
                //--------------------------------------------------------------
                while(sr.size)
                {
                    const claw  &cl = ***(sr.head);
                    const size_t ic = *cl;
                    if( cl.excess(C0, Cs[ic], io) )
                    {
                        //------------------------------------------------------
                        // keep head
                        //------------------------------------------------------
                        cstk.push_back( sr.pop_front() );
                        xs[ic] = io.get();
                        YACK_XMLOG(xml," (+) " << std::setw(15) << xs[ic] << " @" << cl);
                    }
                    else
                    {
                        //------------------------------------------------------
                        // discard head
                        //------------------------------------------------------
                        sr.cut_head();
                    }
                }

                sr.swap_with(cstk);

                if(sr.size<=0) return true;


            }



        }


    }

}
