
#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/look-for.hpp"
#include "yack/sort/sum.hpp"

#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double plexus:: move(const double g0)
        {
            plexus         &self  = *this;                // functionoid
            double          scale = 1.0;                  // for step
            const size_t    count = truncation(scale);    // number of truncated dC
            triplet<double> x     = { 0,  0, 0 };         //
            triplet<double> g     = { g0, 0, 0 };         //

            if(count)
            {
                //--------------------------------------------------------------
                //
                //
                YACK_CHEM_PRINTLN("// [limited]");
                //
                //
                //--------------------------------------------------------------
                static const double xmax = 2.0;

                if(scale<=xmax)
                {
                    //save_profile("lim.dat",scale);
                    g.c = computeVariance( make_trial(x.c=scale,ustack) );
                    minimize::find<double>::run_for(self,x,g,minimize::inside);
                    if(g.b>=scale)
                    {
                        YACK_CHEM_PRINTLN("// [limited@" << scale << " <= 1] maximum: recomputing");
                        g.b = computeVariance( make_trial(x.b=scale,ustack) );
                    }
                }
                else
                {
                    //save_profile("lim.dat",xmax);
                    g.c = self(x.c=xmax);
                    minimize::find<double>::run_for(self,x,g,minimize::inside);
                }
                YACK_CHEM_PRINTLN("// [limited@" << scale << " <= 1] g=" << g.b << " @" << x.b);
                
            }
            else
            {
                //--------------------------------------------------------------
                //
                //
                YACK_CHEM_PRINTLN("// [unlimited]");
                //
                //
                //--------------------------------------------------------------
                //save_profile("ulim.dat",1);
                g.c = g.b = self(x.c = x.b = 1);
                if(g.b>=g.a)
                {
                    //----------------------------------------------------------
                    //
                    YACK_CHEM_PRINTLN("// [unlimited.backtrack]");
                    //
                    //----------------------------------------------------------
                    minimize::find<double>::run_for(self,x,g,minimize::inside);
                    YACK_CHEM_PRINTLN("// [unlimited.backtrack] g=" << g.b << " @" << x.b);

                }
                else
                {
                    //----------------------------------------------------------
                    //
                    YACK_CHEM_PRINTLN("// [unlimited.forward]");
                    //
                    //----------------------------------------------------------
                    assert(g.b<g.a);
                    do
                    {
                        g.c = self( x.c *= 1.2);
                    } while( g.c<g.b );
                    //save_profile("ulim.dat",x.c);
                    minimize::find<double>::run_for(self,x,g,minimize::direct);
                    YACK_CHEM_PRINTLN("// [unlimited.forward] g=" << g.b << " @" << x.b);
                }

            }

            return g.b;
        }
        
    }

}


