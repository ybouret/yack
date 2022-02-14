
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
            double          scale = 1.0;
            const size_t    count = truncation(scale);
            triplet<double> x     = { 0,  0, 0 };
            triplet<double> g     = { g0, 0, 0 };

            if(count)
            {
                //--------------------------------------------------------------
                //
                //
                YACK_CHEM_PRINTLN("// [limited]");
                //
                //
                //--------------------------------------------------------------



                exit(1);
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
                save_profile("ulim.dat",1);
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
                    do
                    {
                        g.c = self( x.c *= 1.2);
                    } while( g.c<g.b );
                    save_profile("ulim.dat",x.c);
                    minimize::find<double>::run_for(self,x,g,minimize::direct);
                    YACK_CHEM_PRINTLN("// [unlimited.forward] g=" << g.b << " @" << x.b);
                }

            }

            return g.b;
        }
        
    }

}


