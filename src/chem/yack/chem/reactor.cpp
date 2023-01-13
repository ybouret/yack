
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
        obs( new sp_repo() ),
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


            for(const snode *sn=lib.head();sn;sn=sn->next) {
                const species &sp = ***sn;
                if(sp.rank<=0) coerce(*obs) << sp;
            }
            YACK_XMLOG(xml, "active    = " << **act);
            YACK_XMLOG(xml, "spectator = " << *obs);


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
#if 0
                    cxx_array<size_t> ranks(M);
                    for(const snode *sn=lib.head();sn;sn=sn->next)
                    {
                        const species &s = ***sn;
                        ranks[*s] = s.rank;
                    }
#endif
                    build_related(xml);
#if 0
                    for(const snode *sn=lib.head();sn;sn=sn->next)
                    {
                        const species &s = ***sn;
                        coerce(s.rank) = ranks[*s];
                    }
#endif
                }

                //--------------------------------------------------------------
                //
                // finalize conservations
                //
                //--------------------------------------------------------------
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

                //--------------------------------------------------------------
                //
                // finalize lattice parameters
                //
                //--------------------------------------------------------------
                YACK_XMLOG(xml, "-- lattice:");
                coerce(L) = all.size();
                YACK_XMLOG(xml, "L  = " << L << " / N = " << N);
                const enode *en = all.head();  for(size_t i=N;i>0;--i) en=en->next;
                coerce(el) = en;

                if(false && verbose) {
                    for(en=el;en;en=en->next) {
                        const equilibrium &eq = ***en;
                        const components  &cm = eq;
                        all.pad(*xml << eq.name, eq) << " : " << cm << std::endl;
                    }
                }

                //--------------------------------------------------------------
                //
                // create squads
                //
                //--------------------------------------------------------------
                create_squads(xml);

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

        size_t reactor:: max_degree() const throw()
        {
            size_t res = 0;
            for(const cluster *cc=linked->head;cc;cc=cc->next) {
                res = max_of(res,cc->cross->size());
            }
            return res;
        }


        namespace
        {
            static inline bool have_common(const readable<int> &lhs,
                                           const readable<int> &rhs) throw()
            {
                assert( lhs.size() == rhs.size() );
                for(size_t i=lhs.size();i>0;--i) {
                    if( (lhs[i] != 0) && (rhs[i] != 0) ) return true;
                }
                return false;
            }

            static inline
            void fill_detached(matrix<bool> &detached, const matrix<int> &Nu ) throw()
            {
                detached.ld(false);
                const size_t r=Nu.rows;
                for(size_t i=1;i<r;++i)
                {
                    const readable<int> &lhs = Nu[i];
                    for(size_t j=i+1;j<=r;++j)
                    {
                        const readable<int> &rhs = Nu[j];
                        detached[i][j] = detached[j][i] = !have_common(lhs,rhs);
                    }
                }
            }
        }

        void reactor:: create_squads(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"squads");
            const size_t dim = all.size();
            matrix<bool> detached(dim,dim);
            matrix<int>  topology(dim,Nu.cols);

            YACK_XMLOG(xml, "-- create global topology");
            for(const enode *en=all.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;
                eq.fill(topology[*eq]);
            }
            fill_detached(detached,topology);
            if(xml.verbose)
            {
                all(*xml << "detached=","",detached);
            }

            for(cluster *cc=linked->head;cc;cc=cc->next)
            {
                cc->make_detached(xml, coerce( *(cc->army) ),detached, "cluster::army");
            }

        }

    }
}
