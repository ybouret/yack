
#include "yack/aqueous/realm/domain.hpp"
#include "yack/aqueous/library.hpp"
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

        static inline string cof2str(const int cf)
        {
            switch(cf)
            {
                case -1: return string('-');
                case  1: return string();
                default:
                    break;
            }
            return vformat("%d*",cf);
        }

        static inline
        string make_eqname(const eq_list       &eqs,
                           const readable<int> &cof)
        {
            assert(cof.size()<=eqs.size);
            string res;
            bool           first = true;
            const eq_node *en    = eqs.head;
            for(size_t ii=1;ii<=cof.size();++ii,en=en->next)
            {
                const int cf = cof[ii];
                if(cf)
                {
                    const equilibrium &eq = ***en;
                    if(first)
                    {
                        res += cof2str(cf) + eq.name;
                        first = false;
                    }
                    else
                    {
                        if(cf>0) res += '+';
                        res += cof2str(cf) + eq.name;
                    }
                }
            }
            return res;
        }

        namespace
        {
            class mixed_equilibrium : public  equilibrium
            {
            public:

                //! parameters to evaluate constant
                class params : public object
                {
                public:
                    const size_t indx0; //!< global index
                    const int    coeff; //!< coefficient
                    params      *next;  //!< for linked

                    inline explicit params(const size_t i,
                                           const int    c) noexcept :
                    object(),
                    indx0(i),
                    coeff(c),
                    next(0)
                    {
                        assert(indx0>0);
                        assert(coeff!=0);
                    }

                    inline virtual ~params() noexcept
                    {
                    }

#if 0
                    inline friend std::ostream & operator<<(std::ostream &os, const params &self)
                    {
                        os << "(" << self.coeff << "@" << self.indx0 << ")";
                        return os;
                    }
#endif
                    
                private:
                    YACK_DISABLE_COPY_AND_ASSIGN(params);
                };

                inline explicit mixed_equilibrium(const string          &uid,
                                                  const size_t           idx,
                                                  const size_t           sub,
                                                  const readable<double> &eks,
                                                  const eq_list          &eqs,
                                                  const readable<int>    &cof) :
                equilibrium(uid,idx),
                K_(eks)
                {
                    coerce(indx[1]) = sub;
                    assert(eqs.size>=cof.size());
                    const size_t   nc = cof.size();
                    const eq_node *en = eqs.head;
                    for(size_t i=1;i<=nc;++i,en=en->next)
                    {
                        assert(en);
                        const int          cf = cof[i]; if(0==cf) continue;
                        const equilibrium &eq = ***en;
                        coerce(par).store( new params(eq.indx[0],cf) );
                    }
                    assert(par.size>=2);
                    coerce(par).reverse();
                    //std::cerr << par << std::endl;
                }

                inline virtual ~mixed_equilibrium() noexcept
                {
                }




            private:
                YACK_DISABLE_COPY_AND_ASSIGN(mixed_equilibrium);
                const readable<double>   &K_;
                const cxx_pool_of<params> par;
                cameo::mul<double>        xmul;

                virtual equilibrium * clone() const { throw exception("mixed_equilibirium is not clonable"); }
                virtual double getK(double)
                {
                    xmul.free();
                    for(const params *p=par.head;p;p=p->next)
                    {
                        xmul.ipower(K_[p->indx0], p->coeff);
                    }
                    return xmul.product();
                }
            };
        }

        void domain:: create_manifold(const xmlog            &xml,
                                      equilibria             &eqs,
                                      const readable<double> &eks)
        {
            //------------------------------------------------------------------
            //
            // create compressed topology
            //
            //------------------------------------------------------------------
            matrix<int> Mu;
            if(N!=raven::qselect::compress(Mu,Nu))
                throw imported::exception(clid,"invalid topology compression");
            YACK_XMLOG(xml,"Mu   = " << Mu);

            //------------------------------------------------------------------
            //
            // use RAVEn
            //
            //------------------------------------------------------------------
            configs conf(*this,live,Nu);
            if(species::verbose) *xml << "RAVEn [";
            {
                raven::qbranch build;
                build.batch(Mu,N,configs::confirm,conf);
            }
            if(species::verbose) xml() << "]" << std::endl;

            //------------------------------------------------------------------
            //
            // prepare equilibri[um|a]
            //
            //------------------------------------------------------------------
            conf.sort();

            for(const config *cfg=conf.head;cfg;cfg=cfg->next)
            {
                //--------------------------------------------------------------
                // gathering parameters
                //--------------------------------------------------------------
                const readable<int> &w   = cfg->weight;
                const string         uid = make_eqname(*this,w);
                const size_t         idx = eqs.next_indx();
                const size_t         sub = size+1;

                //--------------------------------------------------------------
                // create and register mixed_equilibrium
                //--------------------------------------------------------------
                equilibrium  &meq = eqs( new mixed_equilibrium(uid,idx,sub,eks,*this,w) );
                (*this) << meq;
                assert( meq.indx[0] == eqs->size );
                assert( meq.indx[1] == size );

                //--------------------------------------------------------------
                // fill in species with stoich
                //--------------------------------------------------------------
                {
                    const sp_node *sn = live.head;
                    for(size_t j=1;j<=M;++j,sn=sn->next)
                    {
                        const int      nu = cfg->stoich[j]; if(0 == nu) continue;
                        const species &sp = (***sn);        assert(sp.indx[1]==j);
                        meq(nu,sp);
                    }
                    if(!meq.is_neutral()) throw imported::exception(domain::clid,"<%s> is not neutal!!",meq.name());
                }
            }

            //------------------------------------------------------------------
            //
            // update last for originals equilibria
            //
            //------------------------------------------------------------------
            {
                const eq_node *node = head;
                for(size_t i=N;i>0;--i) node=node->next;
                coerce(last) = node;
            }

            coerce(L) = size;

            if(species::verbose)
            {
                *xml << "-------- original --------" << std::endl;
                for(const eq_node *node=head;node!=last;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const components  &cc = eq;
                    eqs.pad(*xml<<eq,eq) << " : " << cc << std::endl;
                }

                *xml << "-------- combined --------" << std::endl;
                const config      *cfg=conf.head;
                for(const eq_node *node=last;node;node=node->next,cfg=cfg->next)
                {
                    const equilibrium &eq = ***node;
                    const components  &cc = eq;
                    eqs.pad(*xml<<eq,eq) << " : " << cc << " | " << cfg->missed << std::endl;
                }
            }
            YACK_XMLOG(xml,"#config=" << conf.size << "+" << N << " => " << L);



        }

    }

}


