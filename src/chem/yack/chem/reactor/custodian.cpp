
#include "yack/chem/reactor/custodian.hpp"
#include "yack/math/iota.hpp"
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

        custodian:: ~custodian() throw()
        {
            
        }

        void custodian:: reset_all() throw()
        {
            sr.clear();
            xs.ld(0);
            dC.ld(0);
            Cs.ld(0);
        }

        bool custodian:: corrected(writable<double> &C0)
        {
            assert(C0.size()>=cs.M);
            dC.ld(0);
            bool did = false;
            for(const cluster *cc=cs.linked->head;cc;cc=cc->next)
            {
                for(const cl_group *cg=cc->clamp->head;cg;cg=cg->next)
                {
                    if( corrected(C0,*cg) ) {
                        did = true;
                    }
                }
            }
            return did;
        }


        bool custodian:: corrected(writable<double> &C0,
                                   const cl_group   &cg)
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
                    std::cerr << " (+) " << std::setw(15) << xs[ic] << " @" << cl << std::endl;
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

                    std::cerr << " (*) " << std::setw(15) << tiny << " @" << *best << std::endl;

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
                    std::cerr << "dC is now " << dC << std::endl;

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
                        std::cerr << " (+) " << std::setw(15) << xs[ic] << " @" << cl << std::endl;
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
