
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
            plexus         &self  = *this;                
            double          scale = 1;
            const size_t    count = truncation(scale);
            triplet<double> x     = { 0,  0, 0 };
            triplet<double> g     = { g0, 0, 0 };
            if(count>0)
            {
                //--------------------------------------------------------------
                //
                YACK_CHEM_PRINTLN("// [limited@" << scale << "]");
                //
                //------------------------------------------------------------------
                //save_profile("lim.dat",min_of(scale,2.0));
                if(scale <= 1)
                {
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// [limited@" << scale << " <= 1]");
                    //----------------------------------------------------------
                    g.c = computeVariance( make_trial(x.c=scale,ustack) );
                    minimize::find<double>::run_for(self,x,g,minimize::inside);
                    if(x.b>=scale)
                    {
                        // recompute
                        g.b = computeVariance( make_trial(x.b=scale,ustack) );
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// [limited@" << scale << " >  1]");
                    //----------------------------------------------------------
                    g.c=g.b=self(x.b=x.c=1);
                    if(g.c>=g.a)
                    {
                        YACK_CHEM_PRINTLN("// |_backtrack");
                        minimize::find<double>::run_for(self,x,g,minimize::inside);
                    }
                    else
                    {
                        YACK_CHEM_PRINTLN("// |_accept");
                    }
                }

            }
            else
            {
                //--------------------------------------------------------------
                //
                YACK_CHEM_PRINTLN("// [unlimited]");
                //
                //--------------------------------------------------------------
                g.c = self(x.c=1);
                if(g.c<g.a)
                {
                    //save_profile("ulim.dat",1.2);
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// [unlimited] forward");
                    //----------------------------------------------------------
                    g.b = self( x.b = 0.5);
                    minimize::find<double>::run_for(self,x,g,minimize::expand);
                }
                else
                {
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// [unlimited] backtrack");
                    //----------------------------------------------------------
                    //save_profile("ulim.dat",1);
                    minimize::find<double>::run_for(self,x,g,minimize::inside);
                }

            }
            return g.b;
        }
        
    }

}


