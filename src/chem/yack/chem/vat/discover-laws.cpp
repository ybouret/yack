#include "yack/chem/vat/cluster.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/system/imported.hpp"
#include "yack/woven/qcompress.hpp"
#include "yack/woven/qbuilder.hpp"

namespace yack
{
    namespace Chemical
    {

        void Cluster:: discoverLaws(const xmlog &xml)
        {
            const size_t nc = active.size;
            if(nc<=1) return;
            YACK_XMLSUB(xml,"Cluster::discoverLaws");
            woven::zrepo     repo(nc);

            // using WOVEn to build possible laws
            {
                matrix<int>  Q(nc,nc);
                if(!math::ortho_family::build(Q,Nu,true))
                    throw imported::exception(CLID,"singular topology!!");

                matrix<int>  P;
                const size_t rank = woven::qcompress::build(P,Q);
                if(rank<=0)
                    throw imported::exception(CLID,"unexpected null rank");

                woven::qbuilder  work(nc);
                work(repo,P,rank,true);
            }

            // process repo
            typedef core_repo<const woven::zvector> zvecs;
            zvecs good;
            for(const woven::zvector *v=repo.head;v;v=v->next)
            {
                size_t nCoef = 0;
                bool   isPos = true;
                for(size_t i=v->size();i>0;--i)
                {
                    switch( (*v)[i].s )
                    {
                        case __zero__:          continue;
                        case positive: ++nCoef; continue;
                        case negative: isPos = false;
                            break;
                    }
                    break;
                }
                if(!isPos||nCoef<=1)  continue;
                good << *v;
            }

            if(good.size<=0)
            {
                YACK_XMLOG(xml,"no possible conservation");
            }

            const size_t qn = good.size;
            Qm.make(qn,nc);
            {
                size_t i = 1;
                for(const zvecs::node_type *node=good.head;node;node=node->next,++i)
                {
                    writable<unsigned>  &u = Qm[i];
                    const readable<apz> &z = (***node);
                    for(size_t j=nc;j>0;--j) u[j] = z[j].cast_to<unsigned>("law coefficient");
                }
            }
            coerce(Qr) = apex::flak::rank(Qm);
            YACK_XMLOG(xml, " Qm = " << Qm);
            YACK_XMLOG(xml, " Qr = " << Qr);

        }

    }

}


