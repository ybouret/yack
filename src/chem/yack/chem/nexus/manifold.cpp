#include "yack/chem/nexus.hpp"
#include "yack/math/iota.hpp"
#include "yack/sequence/bunch.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/raven/qselect.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;
    using namespace raven;

    namespace chemical
    {
        //----------------------------------------------------------------------
        //
        //
        // select species and build matrix from significant rows
        //
        //
        //----------------------------------------------------------------------
        void select_clan(sp_list       &clan,
                         imatrix       &work,
                         const imatrix &mu,
                         const library &lib)
        {
            //------------------------------------------------------------------
            //
            // initialize indices
            //
            //------------------------------------------------------------------
            assert(mu.rows==lib.size());
            assert(0==clan.size);
            const size_t   m = mu.rows;
            vector<size_t> jrow(m,as_capacity);

            //------------------------------------------------------------------
            //
            // find not zero and not propto previously selected
            //
            //------------------------------------------------------------------
            for(const snode *sn=lib.head();sn;sn=sn->next)
            {
                const species       &s = ***sn;
                const size_t         j = *s;
                const readable<int> &curr = mu[j];
                if(qselect::count_valid(curr)<=0) continue;

                bool                 isOk = true;

                for(size_t k=jrow.size();k>0;--k)
                {
                    if( alga::colinear(mu[ jrow[k] ],curr) )
                    {
                        isOk = false;
                        break;
                    }
                }

                if(isOk)
                {
                    jrow << j;
                    clan << &s;
                }
            }

            //------------------------------------------------------------------
            //
            // build sub-matrix
            //
            //------------------------------------------------------------------
            const size_t dims = jrow.size(); assert(dims==clan.size);
            if(dims<=0) throw imported::exception("select sub-manifold species","unexpected zero match!!");

            work.make(dims,mu.cols);
            for(size_t k=dims;k>0;--k)
                iota::load(work[k],mu[ jrow[k] ]);

        }
        

        //----------------------------------------------------------------------
        //
        //
        // convert coefficients
        //
        //
        //----------------------------------------------------------------------
        static const char * const fn = "sub_manifold";


        template <typename T>
        static inline
        bool keep_more_than_two(const readable<T> &cf) throw()
        {
            return qselect::count_valid(cf) >= 2;
        }

        class collector : public bunch<int>
        {
        public:
            explicit collector(const size_t w) : bunch<int>(w) {}
            virtual ~collector() throw() {}

            void operator()(const qvector &cf)
            {
                if( qselect::count_valid(  cf.cast_to(work) ) >= 2 )
                {
                    ensure(work);
                }
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(collector);
        };


        void nexus:: make_manifold_(cluster     &cls,
                                    const xmlog &xml)
        {
            YACK_XMLSUB(xml,fn);
            YACK_XMLOG(xml,cls);

            //------------------------------------------------------------------
            //
            // check local size
            //
            //------------------------------------------------------------------
            const size_t n = cls.size;
            
            if(n<=1) {
                YACK_XMLOG(xml, "<standalone>");
            }
            else
            {

                //--------------------------------------------------------------
                //
                // select local topology
                //
                //--------------------------------------------------------------
                const imatrix mu;
                const sp_list sl;
                {
                    //----------------------------------------------------------
                    //
                    // build local topology
                    //
                    //----------------------------------------------------------
                    imatrix       nu(n,M);
                    {
                        size_t  i=1;
                        for(const eq_node *node=cls.head;node;node=node->next,++i)
                            iota::load(nu[i],Nu[***node]);
                        assert( n == alga::rank(nu) );
                    }
                    //----------------------------------------------------------
                    //
                    // select local vectors: the problem becomes abstract
                    //
                    //----------------------------------------------------------
                    {
                        const imatrix nut(nu,transposed);
                        select_clan(coerce(sl),coerce(mu),nut,corelib);
                        assert( n==alga::rank(nut) );
                        assert( n==alga::rank(mu)  );
                    }

                    if(verbose)
                    {
                        YACK_XMLOG(xml,"-- local topogy");
                        std::cerr << "\tnu=" << nu << std::endl;
                        std::cerr << "\tmu=" << mu << std::endl;
                    }
                }

                //--------------------------------------------------------------
                //
                // build all possible weights
                //
                //--------------------------------------------------------------
                const size_t nd = mu.cols;
                collector    cw(nd);
                {
                    qbranch qgen;
                    qgen.batch(mu,n,keep_more_than_two<int>,cw);
                }


                //--------------------------------------------------------------
                //
                // expanding
                //
                //--------------------------------------------------------------
                cluster        repo;
                {
                    YACK_XMLSUB(xml,"mixing");
                    cxx_array<int> weight(N); // global weight
                    cxx_array<int> stoich(M); // global coefficients
                    unsigned       imix=1;    // mixing index
                    for(const collector::entry *ep=cw->head;ep;ep=ep->next,++imix)
                    {
                        const readable<int> &lcof = *ep;
                        weight.ld(0);
                        size_t  i=1;
                        for(const eq_node *node=cls.head;node;node=node->next,++i)
                        {
                            const size_t ei = ***node;
                            weight[ei] = lcof[i];
                        }

                        qbranch::assess(stoich,weight,Nu);

                        repo << &promote_mixed(weight);

                        if(verbose)
                        {
                            const equilibrium &emix = **repo.tail;
                            const components  &cmix = emix;
                            std::cerr << "  u" << imix << " = " << weight << " => " << stoich << " : " << cmix << " : " << emix.name << std::endl;
                        }
                    }
                }

                YACK_XMLOG(xml,"-- added #" << repo.size << " to local cluster");
                cls.merge_back(repo);
                YACK_XMLOG(xml,"--       #" << cls.size  << " in local cluster");
            }

            //------------------------------------------------------------------
            //
            // create local teams of equilibria
            //
            //------------------------------------------------------------------
            eq_team &roaming =  coerce(cls.genus->roaming);
            eq_team &bounded =  coerce(cls.genus->bounded);

            for(const eq_node *en=cls.head;en;en=en->next)
            {
                const equilibrium &eq = **en;
                switch(eq.kind)
                {
                    case join_only:
                    case part_only:
                        roaming << &eq;
                        break;

                    case both_ways:
                        bounded << &eq;
                        break;

                    case undefined: throw imported::exception(fn,"unexpected undefined <%s>", eq.name());
                }
            }


        }
        
        
        void nexus:: make_manifold(const xmlog &xml)
        {
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);

            // making manifold for each cluster
            for(cluster *sharing=wired.head;sharing;sharing=sharing->next)
                make_manifold_(*sharing,xml);

            YACK_XMLOG(xml, "-- resetting working species");
            assert( worklib.size() == corelib.size() );
            for(const snode *target=worklib.head(), *source=corelib.head();target;target=target->next,source=source->next)
            {
                const species &tgt = ***target;
                const species &src = ***source;
                coerce(tgt.rank) = src.rank;
            }
            YACK_XMLOG(xml, "-- equilibria: from " << singles.size() << "  to " << lattice.size() );

        }
        
    }
    
}

