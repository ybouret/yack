
#include "yack/chem/vat/cluster.hpp"
#include "yack/system/imported.hpp"
#include "yack/woven/qcompress.hpp"
#include "yack/woven/qbuilder.hpp"

namespace yack
{
    namespace Chemical
    {

        void Cluster:: makeManifold(const xmlog &xml)
        {
            const size_t nr = size;
            if(nr<=1) return;
            YACK_XMLSUB(xml,"Cluster::makeManifold");
            {
                matrix<int> Q(Nu,transposed);
                matrix<int> P;
                const size_t rank = woven::qcompress::build(P,Q);
                if(rank!=size)
                    throw imported::exception(CLID,"invalid compressed topology");

                woven::zrepo     repo(nr);
                woven::qbuilder  work(nr);
                work(repo,P,rank,false);
                std::cerr << repo << std::endl;
            }

        }

    }

}


