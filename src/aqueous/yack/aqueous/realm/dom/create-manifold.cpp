
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/raven/qselect.hpp"
#include "yack/math/iota.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack
{
    using namespace math;

    namespace aqueous
    {

        namespace
        {

            typedef cxx_array<int> iarr;

            class ivec : public object, public iarr
            {
            public:
                inline virtual ~ivec() noexcept {}
                inline explicit ivec(const iarr &arr) :
                object(),
                iarr(arr),
                nrm1(__nrm1(arr)),
                next(0),
                prev(0)
                {
                }

                static inline int compare(const ivec *lhs,
                                          const ivec *rhs)
                {
                    switch( __sign::of(lhs->nrm1, rhs->nrm1) )
                    {
                        case negative: return -1;
                        case positive: return  1;
                        case __zero__: break;
                    }
                    return -comparison::lexicographic(*lhs,*rhs);
                }

                const unsigned nrm1;
                ivec *         next;
                ivec *         prev;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(ivec);
                static inline unsigned __nrm1(const iarr &arr) noexcept
                {
                    unsigned sum=0;
                    for(size_t i=arr.size();i>0;--i)
                    {
                        const int x = arr[i];
                        switch(__sign::of(x) )
                        {
                            case __zero__:              continue;
                            case positive: sum += x;    continue;
                            case negative: sum += (-x); continue;
                        }
                    }
                    return sum;
                }
            };

            class ivecs : public cxx_list_of<ivec>
            {
            public:
                const size_t dim;
                iarr         arr;

                inline explicit ivecs(const size_t d) :
                cxx_list_of<ivec>(),
                dim(d),
                arr(d)
                {}

                inline virtual ~ivecs() noexcept
                {
                }

                static inline
                bool confirm(const readable<int> &mu ) noexcept
                {
                    return raven::qselect::count_valid(mu) >= 2;
                }

                void sort()
                {
                    merge_list_of<ivec>::sort(*this, ivec::compare);
                }



                inline
                void operator()( const raven::qvector &cf )
                {
                    //std::cerr << cf << std::endl;
                    assert(dim==cf.size());

                    // convert and count not null coeff
                    arr.ld(0);
                    size_t num = 0;
                    for(size_t i=dim;i>0;--i)
                    {
                        const apq &q = cf[i];
                        if(q.num.s != __zero__)
                        {
                            ++num;
                            arr[i] = q.num.cast_to<int>();
                        }
                    }

                    // exclude not enough coeff
                    if(num<2) return;

                    // exclude multiple coeff
                    for(const ivec *v=head;v;v=v->next)
                    {
                        if( *v == arr ) return;
                    }

                    // keep it
                    push_back( new ivec(arr) );
                    std::cerr << "\t" << *tail << std::endl;
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(ivecs);
            };

        }

        void domain:: create_manifold(const xmlog &xml)
        {
            matrix<int> Mu;
            if(N!=raven::qselect::compress(Mu,Nu)) throw imported::exception(clid,"invalid topology compression");
            YACK_XMLOG(xml,"Mu   = " << Mu);;

            ivecs weights(N);
            {
                raven::qbranch build;
                build.batch(Mu,N,ivecs::confirm,weights);
            }
            weights.sort();
            std::cerr << weights << std::endl;

            cxx_array<int>           coef(M);
            addrbook                 source;
            addrbook                 target;
            solo_repo<const species> missing;

            for(const ivec *v=weights.head;v;v=v->next)
            {
                const readable<int> &weight = *v;
                raven::qbranch::assess(coef,weight,Nu);
                std::cerr << weight << " => " << coef << std::endl;
                source.free();
                target.free();
                for(const eq_node *en=head;en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = eq.indx[1];
                    if(weight[ei]) eq.report_to(source);
                }
                for(const sp_node *sn=live.head;sn;sn=sn->next)
                {
                    const species &sp = ***sn;
                    const size_t   sj = sp.indx[1];
                    if( coef[sj] != 0) target.ensure(&sp);
                }

                std::cerr << "species: " << source->size << " -> " << target->size << std::endl;
                if(target->size >  source->size) throw imported::exception(clid,"invalid equilibria combination!!");
                if(target->size == source->size) continue;;
                missing.clear();
                for(addrbook::const_iterator it=source.begin();it!=source.end();++it)
                {
                    const void *addr = *it;
                    if(target.search(addr)) continue;
                    missing << *static_cast<const species *>(addr);
                }
                std::cerr << " missing: " << missing << std::endl;
            }

        }

    }

}


