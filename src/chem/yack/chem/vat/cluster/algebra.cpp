
#include "yack/chem/vat/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace Chemical
    {

        void Cluster:: apply(const IndexLevel        level,
                             writable<double>       &target,
                             const readable<double> &source,
                             const readable<double> &extent,
                             Extended::Add          &xadd) const
        {
            for(const Species::Node *sn=lib.head;sn;sn=sn->next)
            {
                const Species &sp = ***sn;
                const size_t   sj =sp.indx[level];
                xadd.free();
                xadd.push( source[sj] );
                const readable<int> &nu_j = NuT[ sp.indx[SubLevel] ];
                for(const Equilibrium::Node *en=head;en!=last;en=en->next)
                {
                    const Equilibrium &eq = ***en;
                    const size_t       ei = eq.indx[SubLevel];
                    const double       xi = extent[ei];
                    const double       nu = nu_j[ei];
                    xadd.prod<double,double>(xi,nu);
                }
                target[ sj ] = *xadd;
            }
        }

    }

}


