
#include "yack/chem/nexus.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/kernel.hpp"

namespace yack
{
    namespace chemical
    {

        const char * const nexus:: clid = "[nexus]";
        bool              &nexus::verbose = entity::verbose;


        nexus:: ~nexus() throw()
        {
        }

        nexus:: nexus(const library    &lib,
                      const equilibria &eqs,
                      const double      t) :
        corelib( lib ),
        singles( eqs ),
        working( corelib.head() ),
        M( corelib.size() ),
        N( singles.size() ),
        Nu(N,N>0?M:0),
        worklib( corelib ),

        mtab(12,M),
        ntab(12,N),

        // species
        crit(mtab.next(),transmogrify),

        // eqs
        K( ntab.next() ),
        regular(),
        roaming(),

        lockLib(coerce(corelib)),
        lockEqs(coerce(singles))
        {
            const xmlog xml(clid,std::cerr,verbose);
            YACK_XMLSUB(xml, "initialize");

            assert(corelib.size()==worklib.size());
            for(const snode *sn=corelib.head();sn;sn=sn->next)
            {
                const species &s = ***sn;
                const size_t   j = *s;
                coerce(crit[j]) = (s.rank<=0) ? spectator : conserved;
            }

            // initialize singles, their properties and their
            // impact on species
            if(N>0)
            {
                YACK_XMLSUB(xml, "singles");
                for(const enode *en=singles.head();en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = *eq;
                    YACK_XMLOG(xml,eq);

                    // check validity
                    if(!eq.neutral()) throw imported::exception(clid,"<%s> is not neutral", eq.name() );
                    if(!eq.minimal()) throw imported::exception(clid,"<%s> is not minimal", eq.name() );

                    // prepare topology
                    eq.fill(coerce(Nu[ei]));

                    // classification
                    switch(eq.kind)
                    {
                        case undefined: throw imported::exception(clid,"<%s> is undefined", eq.name() );
                        case both_ways:
                            coerce(regular) << &eq;
                            break;

                        case join_only:
                        case part_only:
                            coerce(roaming) << &eq;
                            for(const cnode *cn=eq.head();cn;cn=cn->next)
                            {
                                const species &s = ****cn;
                                coerce(crit[*s]) = unbounded;
                            }
                            break;
                    }

                    // setup constant
                    K[ei] = eq.K(t);
                }

                // checking rank
                const size_t rank =  apk::gj_rank_of(Nu);
                YACK_XMLOG(xml, "-- Nu   = " << Nu);
                YACK_XMLOG(xml, "-- rank = " << rank << " / " << N);
                if(rank<N) throw imported::exception(clid,"dependent equilibria");

                singles.graphviz("singles.dot", corelib);

                // build related (at least one cluster)
                build_related(xml); assert(related.size>0);

                // make manifold
                make_manifold(xml);
            }

            



        }


    }

}
