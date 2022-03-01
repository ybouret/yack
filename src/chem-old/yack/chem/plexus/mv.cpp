
#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/numeric.hpp"
#include "yack/ios/fmt/align.hpp"

#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double plexus:: move(const double g0)
        {

            double          scale = 1.0;
            const size_t    count = truncation(scale);
            triplet<double> x     = {0,0,0};
            triplet<double> g     = {g0,0,0};

            YACK_CHEM_PRINTLN("//" << std::endl << "//   <move>");
            YACK_CHEM_PRINTLN("//     g0 = " << g0);
            if(count)
            {
                YACK_CHEM_PRINTLN("//     [limited@" << scale << "]");
                if(scale>1)
                {
                    YACK_CHEM_PRINTLN("//     |_@" << scale << " > 1");
                    save_profile("lim.dat",1);
                    g.c=g.b=(*this)(x.c=x.b=1);
                    if(g.c>=g.a)
                    {
                        // backtrack
                        YACK_CHEM_PRINTLN("//     |_backtrack");
                        minimize::find<double>::run_for(*this,x,g,minimize::inside);
                    }
                    else
                    {
                        // accept
                        YACK_CHEM_PRINTLN("//     |_accept");
                    }

                }
                else
                {
                    YACK_CHEM_PRINTLN("//     |_@" << scale << " <= 1");
                    save_profile("lim.dat",scale);
                    g.c=g.b=(x.c=x.b=scale*0.9);
                    if(g.c>=g.a)
                    {
                        // backtrack
                        YACK_CHEM_PRINTLN("//     |_backtrack");
                        minimize::find<double>::run_for(*this,x,g,minimize::inside);
                    }
                    else
                    {
                        // accept
                        YACK_CHEM_PRINTLN("//     |_accept");
                    }
                }

            }
            else
            {
                YACK_CHEM_PRINTLN("//     [unlimited]");
                g.c=g.b=(*this)(x.c=x.b=1);
                save_profile("ulim.dat",1);
                if(g.c>=g.a)
                {
                    // backtrack
                    YACK_CHEM_PRINTLN("//     |_backtrack");
                    minimize::find<double>::run_for(*this,x,g,minimize::inside);
                }
                else
                {
                    // accept
                    YACK_CHEM_PRINTLN("//     |_accept");
                }
            }
            const double g1 = g.b;
            YACK_CHEM_PRINTLN("//     g1 = " << g1 << " / " << g0);
            YACK_CHEM_PRINTLN("//   <move/>");

            return g1;
        }

    }

}
