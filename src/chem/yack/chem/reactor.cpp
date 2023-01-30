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

        reactor:: reactor(library                &lib_,
                          equilibria             &eqs_,
                          const readable<double> &K) :
        lib(lib_),
        eqs(eqs_),
        all(eqs),

        act( new alist(lib.head()) ),
        obs( new sp_repo() ),
        M(lib.size()),
        N(eqs.size()),

        Nu(N,N>0?M:0),
        NuT(Nu,transposed),

        fixed(  new booleans(M,true) ),
        linked( new clusters()       ),

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
                if(sp.rank<=0)
                {
                    coerce(*obs) << sp;
                    coerce((*fixed)[*sp]) = false;
                }
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
                coerce(NuT).assign(Nu,transposed);
                YACK_XMLOG(xml, "NuT=" << NuT);

                //--------------------------------------------------------------
                //
                // build all clusters
                //
                //--------------------------------------------------------------
                build_related(xml,K);

                
                //--------------------------------------------------------------
                //
                // finalize conservations
                //
                //--------------------------------------------------------------
                compile_claws(xml);

                
                //--------------------------------------------------------------
                //
                // finalize lattice parameters
                //
                //--------------------------------------------------------------
                setup_lattice(xml);

                //--------------------------------------------------------------
                //
                // create squads
                //
                //--------------------------------------------------------------
                create_squads(xml);
                YACK_XMLOG(xml, "-- max actors: " << max_actors() );
            }

            if(verbose)
            {
                lib(*xml << "fixed=" , "", *fixed);
            }
        }

        void reactor:: setup_lattice(const xmlog &xml)
        {
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

        size_t reactor:: max_actors() const throw()
        {
            size_t res = 0;
            for(const enode *en=all.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;
                res = max_of(res,eq.reac->size);
                res = max_of(res,eq.prod->size);
            }
            return res;
        }

        size_t reactor:: max_cluster_size() const throw()
        {
            size_t res = 0;
            
            for(const cluster *cc=linked->head;cc;cc=cc->next) {
                res = max_of(res,cc->single->size);
            }
            
            return res;
        }
        
        void reactor:: computeK(writable<double> &K, const double t) const
        {
            assert(K.size()>=L);
            for(const enode *en=all.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;
                K[*eq] = eq.K(t);
            }
        }

       
    }
}
