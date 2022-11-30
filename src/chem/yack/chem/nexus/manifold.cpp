#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/algebra/ortho-family.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        static inline int sp_node_compare(const sp_node *lhs, const sp_node *rhs) throw()
        {
            return comparison::increasing( ***lhs, ***rhs );
        }


        
        class stvec : public object, public ivector
        {
        public:
            stvec(const readable<int> &stoi) :
            ivector(stoi,transmogrify),
            next(0),
            prev(0)
            {
            }

            virtual ~stvec() throw() {}

            inline bool is_opposite(const stvec &rhs) const throw()
            {
                const stvec &lhs = *this;
                assert(lhs.size()==rhs.size());
                for(size_t j=lhs.size();j>0;--j)
                {
                    if(lhs[j]!=-rhs[j]) return false;
                }
                return true;
            }

            inline bool is_same_than(const stvec &rhs) const throw()
            {
                const stvec &lhs = *this;
                assert(lhs.size()==rhs.size());
                for(size_t j=lhs.size();j>0;--j)
                {
                    if(lhs[j]!=rhs[j]) return false;
                }
                return true;
            }

            inline friend bool operator==(const stvec &lhs, const stvec &rhs) throw()
            {
                assert(lhs.size()==rhs.size());
                return lhs.is_same_than(rhs) || lhs.is_opposite(rhs);
            }


            stvec *next;
            stvec *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(stvec);
        };

        typedef cxx_list_of<stvec> stpool_;

        class stpool : public stpool_
        {
        public:
            explicit stpool() throw() : stpool_() {}
            virtual ~stpool() throw() {}

            bool add(const readable<int> &stoi) throw()
            {
                auto_ptr<stvec> lhs( new stvec(stoi) );
                for(const stvec *rhs=head;rhs;rhs=rhs->next)
                {
                    if(*lhs == *rhs)
                    {
                        std::cerr << rhs << " already present" << std::endl;
                        return false;
                    }

                }
                push_back( lhs.yield() );
                return true;
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(stpool);
        };


        void nexus:: make_manifold(const xmlog &xml)
        {
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);

            //------------------------------------------------------------------
            //
            // outer loop: use a cluster of related equilibria
            //
            //------------------------------------------------------------------
            addrbook   tribe; // to be populated by eqs
            sp_repo    cache; // from tribe
            const apq _0 = 0;
            const apq _1 = 1;

            for(const cluster *sharing=related.head;sharing;sharing=sharing->next)
            {
                YACK_XMLSUB(xml, "sub_manifold");
                YACK_XMLOG(xml,*sharing);

                const size_t n = sharing->size;
                if(n<=1)
                {
                    continue;
                }

                vector<equilibrium *> eptr(n,as_capacity); //!< inside this sharing
                vector<int>           stoi(M);             //!< resulting

                for(const eq_node *en=sharing->head;en;en=en->next)
                {
                    const equilibrium &eq = **en;
                    eptr.push_back( & coerce(eq) );
                }




                for(size_t k=2;k<=n;++k)
                {
                    combination           comb(n,k);  // combination
                    vector<equilibrium *> esub(k);    // sub-equilibria
                    vector<int>           coef(k);    // shared coefficient


                    do
                    {
                        cache.free();

                        //------------------------------------------------------
                        // create sub equilibria
                        //------------------------------------------------------
                        comb.extract(esub,eptr);

                        //------------------------------------------------------
                        // extract all species, sorted by index
                        //------------------------------------------------------
                        tribe.free();
                        for(size_t i=k;i>0;--i)
                            esub[i]->update(tribe);

                        for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                        {
                            cache.push_back( *static_cast<const species *>( *it ) );
                        }
                        merge_list_of<sp_node>::sort(cache.list,sp_node_compare);

                        const size_t m = cache->size; assert(m>0);

                        //------------------------------------------------------
                        // extract sub-matrix with rank=k and m species
                        //------------------------------------------------------
                        imatrix nu(k,m);
                        imatrix mu(m,k);
                        for(size_t i=k;i>0;--i)
                        {
                            const size_t         ei   = **esub[i];
                            const readable<int> &topo = Nu[ei];
                            size_t               j    = 1;
                            for(const sp_node *sn=cache->head;sn;sn=sn->next,++j)
                            {
                                mu[j][i] = nu[i][j] = topo[***sn];
                            }
                        }
                        //assert( apk::gj_rank_of(nu) == k);
                        //assert( apk::gj_rank_of(mu) == k);


                        if(verbose) {
                            *xml << "-- [";
                            for(size_t i=1;i<=k;++i)
                                std::cerr << ' '  << esub[i]->name;
                            std::cerr << " ] / "  << cache.list << std::endl;
                            *xml << "   |_nu=" << nu << std::endl;
                            *xml << "   |_mu=" << mu << std::endl;

                        }



                        
                        


                    }
                    while(comb.next());
                }

                //std::cerr << stcof << std::endl;

            }




        }

    }

}

