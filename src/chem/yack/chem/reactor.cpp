
#include "yack/chem/reactor.hpp"
#include "yack/apex/alga.hpp"
#include "yack/system/imported.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace chemical
    {
        reactor:: ~reactor() throw()
        {
        }



        const char * const reactor:: clid    = "[reactor]";
        bool &             reactor:: verbose = entity::verbose;

        reactor:: reactor(library                &lib_,
                          equilibria             &eqs_,
                          const readable<double> &K_) :
        lib(lib_),
        eqs(eqs_),
        all(eqs),
        K(K_),

        act( new alist(lib.head()) ),
        
        M(lib.size()),
        N(eqs.size()),

        Nu(N,N>0?M:0),

        linked( new clusters() ),

        Nq(0),
        Qm(),

        L(0),
        el(0),

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
                //--------------------------------------------------------------
                //
                // build global topology
                //
                //--------------------------------------------------------------
                for(const enode *en=eqs.head();en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = *eq;
                    eq.fill(coerce(Nu)[ei]);
                    if(!eq.neutral()) throw imported::exception(clid,"<%s> is not neutral", eq.name() );
                    if(!eq.minimal()) throw imported::exception(clid,"<%s> is not minimal", eq.name() );
                }

                //--------------------------------------------------------------
                //
                // check global topology
                //
                //--------------------------------------------------------------
                YACK_XMLOG(xml, "Nu=" << Nu);
                const size_t rank = alga::rank(Nu);
                if(rank<N) throw imported::exception(clid,"only %u independent equilibria/%u", unsigned(rank), unsigned(N) );

                //--------------------------------------------------------------
                //
                // build all clusters
                //
                //--------------------------------------------------------------
                {
                    cxx_array<size_t> ranks(M);
                    for(const snode *sn=lib.head();sn;sn=sn->next)
                    {
                        const species &s = ***sn;
                        ranks[*s] = s.rank;
                    }
                    build_related(xml);
                    for(const snode *sn=lib.head();sn;sn=sn->next)
                    {
                        const species &s = ***sn;
                        coerce(s.rank) = ranks[*s];
                    }
                }

                // finalize
                for(const cluster *cc=linked->head;cc;cc=cc->next)
                    coerce(Nq) += cc->canon->size;

                YACK_XMLOG(xml, "-- conservations:");
                YACK_XMLOG(xml, "Nq = " << Nq);

                if(Nq)
                {
                    matrix<unsigned>  &Q = coerce(Qm); Q.make(Nq,M);
                    size_t             i = 0;
                    for(const cluster *cc=linked->head;cc;cc=cc->next)
                    {
                        for(const claw *cl = cc->canon->head;cl;cl=cl->next)
                        {
                            writable<unsigned> &q = Q[++i];
                            for(const actor *a = (*cl)->head; a; a=a->next)
                            {
                                q[***a] = a->nu;
                            }
                        }
                    }
                    YACK_XMLOG(xml, "Qm = " << Qm);
                }

                YACK_XMLOG(xml, "-- lattice:");
                coerce(L) = all.size();
                YACK_XMLOG(xml, "L  = " << L);
                const enode *en = all.head();  for(size_t i=N;i>0;--i) en=en->next;
                coerce(el) = en;
                if(verbose) {
                    for(en=el;en;en=en->next) {
                        const equilibrium &eq = ***en;
                        const components  &cm = eq;
                        all.pad(*xml << eq.name, eq) << " : " << cm << std::endl;
                    }
                }
            }
        }


        size_t reactor:: max_claws_per_cluster() const throw()
        {
            size_t res = 0;
            for(const cluster *cc=linked->head;cc;cc=cc->next) {
                res = max_of(res,cc->canon->size);
            }
            return res;
        }


    }
}
