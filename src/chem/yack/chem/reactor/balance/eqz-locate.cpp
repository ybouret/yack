#include "yack/chem/reactor/balance/equalizer.hpp"


namespace yack {

    using namespace math;

    namespace chemical
    {
        void equalizer:: locate_single_fence(frontier        &sf,
                                             const frontier  &limiting,
                                             const frontiers &amending,
                                             const xmlog     &xml)
        {
            static const char pfx[] = " | --> ";

            YACK_XMLOG(xml, pfx  << "locate single fence in " << amending);
            assert(amending.size()>0);
            assert(amending.are_increasing());
            sf.free();
            if(limiting.size<=0)
            {
                //--------------------------------------------------------------
                //
                // take the biggest amending!
                //
                //--------------------------------------------------------------
                sf =  amending.back();
                YACK_XMLOG(xml, pfx << "greatest at " << sf);
            }
            else
            {
                //--------------------------------------------------------------
                //
                // generic case: test w.r.t front amending
                //
                //--------------------------------------------------------------
                const size_t n  =  amending.size();
                const double xx =  limiting.xi;
                switch( __sign::of(xx,amending.front().xi) )
                {
                    case negative:
                        sf = limiting;
                        YACK_XMLOG(xml,pfx << "best effort " << sf << " [won't solve]");
                        return;

                    case __zero__:
                        (sf = limiting) += amending.front();
                        YACK_XMLOG(xml,pfx << "match first " << sf);
                        return;

                    case positive:
                        if(n<=1)
                        {
                            sf =  amending.front();
                            YACK_XMLOG(xml,pfx << "solve last  " << sf << " [unique species]");
                            return;
                        }
                        break;
                }

                assert(amending.size()>=2);

                //--------------------------------------------------------------
                //
                // generic case: test w.r.t back amending
                //
                //--------------------------------------------------------------
                switch( __sign::of(xx,amending.back().xi) )
                {
                    case positive:
                        sf = amending.back();
                        YACK_XMLOG(xml,pfx << "solve last  " << sf);
                        return;

                    case __zero__:
                        (sf = limiting) +=  amending.back();
                        YACK_XMLOG(xml,pfx << "match last  " << sf);
                        return;

                    case negative:
                        break;
                }

                assert(xx>amending.front().xi);
                assert(xx<amending.back().xi);

                size_t jlo = 1;
                size_t jup = n;

            LOOK_UP:
                assert(xx>amending[jlo].xi);
                assert(xx<amending[jup].xi);
                if(jup-jlo<=1)
                {
                    sf = amending[jlo];
                    YACK_XMLOG(xml,pfx << "solve core  " << sf << " #" << jlo) ;
                    return;
                }

                const size_t     jnx = jlo+1; assert(jnx<=jup);
                const frontier  &af  = amending[jnx];
                switch( __sign::of(xx,af.xi) )
                {
                    case negative:
                        sf = amending[jlo];
                        YACK_XMLOG(xml,pfx << "solve core  " << sf << " #" << jlo) ;
                        return;

                    case __zero__:
                        (sf = limiting) += af;
                        YACK_XMLOG(xml,pfx << "match core  " << sf << " #" << jnx);
                        break;

                    case positive:
                        jlo=jnx;
                        goto LOOK_UP;
                }




            }

        }
    }

}


