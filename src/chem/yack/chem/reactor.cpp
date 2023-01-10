
#include "yack/chem/reactor.hpp"
#include "yack/apex/alga.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {
        reactor:: ~reactor() throw()
        {
        }



        const char * const reactor:: clid    = "[reactor]";
        bool &             reactor:: verbose = entity::verbose;

        reactor:: reactor(library     &lib_,
                          equilibria  &eqs_,
                          const double t0) :
        lib(lib_),
        eqs(eqs_),
        
        act( new alist(lib.head()) ),
        
        M(lib.size()),
        N(eqs.size()),

        Nu(N,N>0?M:0),

        linked( new clusters() ),

        libLock(lib_),
        eqsLock(eqs_)
        {
            const xmlog xml(clid,std::cerr,verbose);
            YACK_XMLSUB(xml,"initialize");

            {
                YACK_XMLSUB(xml, "library");
                YACK_XMLOG(xml,lib);
            }

            {
                YACK_XMLSUB(xml, "equilibria");
                YACK_XMLOG(xml,eqs);
            }

            YACK_XMLOG(xml, "active=" << **act);

            if(N>0)
            {
                // build global topology
                for(const enode *en=eqs.head();en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = *eq;
                    eq.fill(coerce(Nu)[ei]);
                }
                YACK_XMLOG(xml, "Nu=" << Nu);
                const size_t rank = alga::rank(Nu);
                if(rank<N) throw imported::exception(clid,"only %u independent equilibria/%u", unsigned(rank), unsigned(N) );
                
                build_related(xml);
                
            }
        }

    }
}
