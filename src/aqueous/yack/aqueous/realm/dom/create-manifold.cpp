
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
            typedef cxx_array<int>  coefficients;

            class config : public object
            {
            public:
                inline config(const coefficients &cf,
                              const coefficients &nu) :
                weight(cf),
                stoich(nu),
                wnorm1(__norm1(weight)),
                missed(),
                next(0),
                prev(0)
                {
                }

                static inline int compare(const config *lhs,
                                          const config *rhs)
                {
                    switch( __sign::of(lhs->wnorm1, rhs->wnorm1) )
                    {
                        case negative: return -1;
                        case positive: return  1;
                        case __zero__: break;
                    }
                    return -comparison::lexicographic(lhs->weight,rhs->weight);
                }

                const coefficients   weight;
                const coefficients   stoich;
                const unsigned       wnorm1;
                const sp_list        missed;
                config              *next;
                config              *prev;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(config);
                static inline unsigned __norm1(const coefficients &arr) noexcept
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

            typedef cxx_list_of<config> configs_;

            class configs : public configs_
            {
            public:
                const eq_list           &elist;   // persistent
                const sp_list           &slist;   // persistent
                const matrix<int>       &Nu;      // persistent
                const size_t             n;       // |elist|
                const size_t             m;       // |slist|
                coefficients             weight;  // weigth[n]
                coefficients             stoich;  // stoich[m]
                addrbook                 source;  // for species
                addrbook                 target;  // for species
                solo_repo<const species> missing; // species


                inline configs(const eq_list     &usr_eq,
                               const sp_list     &usr_sp,
                               const matrix<int> &usr_Nu) :
                configs_(),
                elist(usr_eq),
                slist(usr_sp),
                Nu(usr_Nu),
                n(elist.size),
                m(slist.size),
                weight(n),
                stoich(m),
                source(),
                target(),
                missing()
                {
                }

                inline virtual ~configs() noexcept {}



                static inline
                bool confirm(const readable<int> &mu ) noexcept
                {
                    return raven::qselect::count_valid(mu) >= 2;
                }

                void sort()
                {
                    merge_list_of<config>::sort(*this, config::compare);
                }

                inline
                void operator()( const raven::qvector &cf )
                {
                    assert(n==cf.size());

                    //----------------------------------------------------------
                    // convert and count not null coeff
                    //----------------------------------------------------------
                    weight.ld(0);
                    size_t num = 0;
                    for(size_t i=n;i>0;--i)
                    {
                        const apq &q = cf[i];
                        if(q.num.s != __zero__)
                        {
                            ++num;
                            weight[i] = q.num.cast_to<int>();
                        }
                    }

                    //----------------------------------------------------------
                    // exclude not enough coeff
                    //----------------------------------------------------------
                    if(num<2) return;

                    //----------------------------------------------------------
                    // compute stoich
                    //----------------------------------------------------------
                    raven::qbranch::assess(stoich,weight,Nu);

                    //----------------------------------------------------------
                    // exclude multiple weight/stoich
                    //----------------------------------------------------------
                    for(const config *v=head;v;v=v->next)
                    {
                        if( v->weight == weight ) return;
                        if( v->stoich == stoich ) return;
                    }

                    //----------------------------------------------------------
                    // record source species
                    //----------------------------------------------------------
                    source.free();
                    for(const eq_node *en=elist.head;en;en=en->next)
                    {
                        const equilibrium &eq = ***en;
                        const size_t       ei = eq.indx[1];
                        if(weight[ei]) eq.report_to(source);
                    }

                    //----------------------------------------------------------
                    // record target species
                    //----------------------------------------------------------
                    target.free();
                    for(const sp_node *sn=slist.head;sn;sn=sn->next)
                    {
                        const species &sp = ***sn;
                        const size_t   sj = sp.indx[1];
                        if( 0!= stoich[sj] ) target.ensure(&sp);
                    }

                    switch( __sign::of(target->size, source->size) )
                    {
                        case positive: throw imported::exception(domain::clid,"invalid equilibria combination!!");
                        case __zero__: return; // blank
                        case negative:
                            break;
                    }

                    //----------------------------------------------------------
                    // record target species
                    //----------------------------------------------------------
                    missing.clear();
                    for(addrbook::const_iterator it=source.begin();it!=source.end();++it)
                    {
                        const void *addr = *it;
                        if(target.search(addr)) continue;
                        missing << *static_cast<const species *>(addr);
                    }

                    if(missing.size!=source->size-target->size)
                        throw imported::exception(domain::clid,"invalid missing count!!");



                    //----------------------------------------------------------
                    // keep it
                    //----------------------------------------------------------
                    coerce(push_back( new config(weight,stoich) )->missed).swap_with(missing);
                    if(species::verbose) (std::cerr << '.').flush();
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(configs);
            };

        }

        void domain:: create_manifold(const xmlog &xml)
        {
            matrix<int> Mu;
            if(N!=raven::qselect::compress(Mu,Nu)) throw imported::exception(clid,"invalid topology compression");
            YACK_XMLOG(xml,"Mu   = " << Mu);;

            configs conf(*this,live,Nu);
            if(species::verbose) xml() << " [";
            {
                raven::qbranch build;
                build.batch(Mu,N,configs::confirm,conf);
            }
            if(species::verbose) std::cerr << "]" << std::endl;

            std::cerr << "#config=" << conf.size << std::endl;
            conf.sort();
            for(const config *cfg=conf.head;cfg;cfg=cfg->next)
            {
                std::cerr << cfg->weight << " => " << cfg->stoich << " => " << cfg->missed << std::endl;
            }



        }

    }

}


