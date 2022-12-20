
#include "yack/chem/nexus.hpp"
#include "yack/math/iota.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/sequence/bunch.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;
    using namespace raven;

    namespace chemical
    {

        namespace {

            template <typename T>
            static inline
            void compressQ(matrix<T> &Q, const matrix<T> &Q0)
            {
                const size_t       n = Q0.rows;
                cxx_series<size_t> indx(n);

                for(size_t i=1;i<=n;++i)
                {
                    const readable<T>   &Qi = Q0[i];
                    bool                 ok = true;
                    for(size_t j=indx.size();j>0;--j)
                    {
                        if( apk::are_prop(Qi,Q0[indx[j]],NULL))
                        {
                            ok = false;
                            break;
                        }
                    }
                    if(!ok) continue;
                    indx << i;
                }
                const size_t q = indx.size();
                Q.make(q,Q0.cols);
                for(size_t j=1;j<=q;++j)
                    iota::load(Q[j],Q0[indx[j]]);
            }




            class collector : public bunch<int>
            {
            public:
                explicit collector(const size_t dim) : bunch<int>(dim)
                {
                }

                virtual ~collector() throw()
                {
                }

                bool validate(const readable<apz> &v) const throw()
                {
                    assert(v.size()==width);
                    size_t np = 0;
                    for(size_t i=width;i>0;--i)
                    {
                        switch(v[i].s)
                        {
                            case __zero__:
                                continue;
                            case positive:
                                ++np;
                                continue;
                            case negative:
                                return false;
                        }
                    }
                    return np>1;
                }

                void operator()(const qvector &v)
                {
                    const bool ok = validate(v) && insert( v.cast_to<int>(work) );
                    if(ok)
                    {
                        std::cerr << "[+] " << v << std::endl;
                    }

                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(collector);
            };


        }

        void nexus:: conserved_set_(cluster &sharing, const xmlog &xml)
        {
            static const char * const fn = "conserved_subset";
            YACK_XMLSUB(xml,fn);
            eq_team      usual;
            sp_list      house;

            //------------------------------------------------------------------
            //
            // extract usual equilibria and house of species
            //
            //------------------------------------------------------------------
            {
                addrbook     tribe;

                for(const eq_node *en=sharing.head;en;en=en->next)
                {
                    const equilibrium &eq = **en;
                    if( regular.met(eq) )
                    {
                        usual << &eq;
                        eq.update(tribe);
                    }
                }

                for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                {
                    const species &sp = *static_cast<species*>(*it);
                    if( conserved != crit[*sp] ) continue;;
                    house << static_cast<species *>(*it);
                }
                house.sort();
            }

            //------------------------------------------------------------------
            //
            // study result
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- #usual = " << usual.size << " / " << sharing.size << ", #house=" << house.size);
            if(usual.size<=0) return;
            if(verbose)
            {
                for(const eq_node *node=usual.head;node;node=node->next)
                {
                    *xml << " (*) " << (**node).name << std::endl;
                }
                *xml << "  |_" << house << std::endl;
            }
            assert(house.size>=2);


            const size_t n = usual.size;
            const size_t m = house.size;
            switch( __sign::of(m,n) )
            {
                case negative: throw imported::exception(fn,"too few species");
                case __zero__:
                    YACK_XMLOG(xml, "-- specific system");
                    return;
                case positive:
                    break;
            }

            //------------------------------------------------------------------
            //
            // build virtual topology
            //
            //------------------------------------------------------------------
            assert(n<m);
            const size_t ker = m-n;
            imatrix      nu(n,m);
            {
                size_t i=1;
                for(const eq_node *en=usual.head;en;en=en->next,++i)
                {
                    const readable<int> &Nu_i = Nu[***en];
                    writable<int>       &nu_i = nu[i];
                    size_t j=1;
                    for(const sp_node *sn=house.head;sn;sn=sn->next,++j) {
                        nu_i[j] = Nu_i[ ***sn ];
                    }
                }
            }
            std::cerr << "nu=" << nu << std::endl;

            imatrix Q;
            {
                imatrix Q0(m,m);
                if(!ortho_family::construct(Q0,nu)) throw imported::exception(fn,"singular sub-system");
                compressQ(Q,Q0); std::cerr << "Q=" << Q << std::endl;
                if(ker!=apk::rank(Q))          throw imported::exception(fn,"singular compressed sub-system");
            }

            collector cb(m);
            {
                qbranch  source;
                source.batch(Q,ker,cb);
            }



        }


        void nexus:: conserved_set(const xmlog &xml)
        {
            static const char * const fn = "conserved_set";
            YACK_XMLSUB(xml,fn);
            for(cluster *sharing=related.head;sharing;sharing=sharing->next)
            {
                conserved_set_(*sharing,xml);
            }
        }

        
    }
    
}

