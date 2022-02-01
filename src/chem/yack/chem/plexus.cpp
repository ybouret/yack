

#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        plexus:: ~plexus() throw() {}

        static const char fn[] = "chemical::plexus";

        static inline
        size_t check(const library &lib, const equilibria &eqs)
        {
            if(lib.size()<eqs.size())
            {
                throw exception("%s: too many equilibria",fn);
            }
            return lib.size();
        }

        plexus:: plexus(library &lib_, equilibria &eqs_) :
        lib(lib_),
        eqs(eqs_),
        N(eqs.size()),
        M(check(lib,eqs)),
        ntab(10,N),
        mtab(10,M),
        K(ntab.next()),
        Nu(N, (N>0) ? M : 0),
        NuT(Nu.cols,Nu.rows),
        lib_lock( coerce(lib) )
        {

            // initialize Nu
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq.fill(coerce(Nu[eq.indx]));
            }

            // finalize
            coerce(NuT).assign(Nu,transposed);

            if(N>0)
            {
                matrix<apq> G(N,N);
                tao::v3::gram(G,Nu);
                std::cerr << "Nu=" << Nu << std::endl;
                std::cerr << "NuT=" << NuT << std::endl;
                std::cerr << "G=" << G << std::endl;
            }

        }

        void plexus:: computeK(const double t)
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                K[eq.indx] = eq.K(t);
            }
        }
    }
    
}

