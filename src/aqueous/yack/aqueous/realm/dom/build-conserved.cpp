
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/raven/qselect.hpp"
#include "yack/math/iota.hpp"


namespace yack
{
    namespace aqueous
    {

        void build_Q0(matrix<int>       &Q0,
                      const matrix<int> &Q)
        {
            const size_t   nr = Q.rows;
            vector<size_t> ir(nr,as_capacity);
            for(size_t i=1;i<=Q.rows;++i)
            {
                bool ok = true;
                const readable<int> &r = Q[i];
                for(size_t j=ir.size();j>0;--j)
                {
                    if( alga::colinear(r,Q[ir[j]]) )
                    {
                        ok = false;
                        break;
                    }
                }
                if(ok) ir << i;
            }
            std::cerr << "indices=" << ir << std::endl;
            const size_t nq = ir.size();
            Q0.make(nq,Q.cols);
            for(size_t i=ir.size();i>0;--i)
            {

            }
        }

        namespace
        {

            typedef cxx_array<unsigned> uarr;

            class uvec : public object, public uarr
            {
            public:
                inline explicit uvec(const uarr &arr) :
                object(),
                uarr(arr),
                nrm1(norm(*this)),
                next(0),
                prev(0)
                {
                }

                inline virtual ~uvec() noexcept {}

#if 0
                inline friend
                std::ostream & operator<<(std::ostream &os, const uvec &self)
                {
                    const uarr &a = self;
                    os << '|' << a  << '|' << '=' << self.nrm1;
                    return os;
                }
#endif

                inline static int compare(const uvec *lhs,
                                          const uvec *rhs) noexcept
                {
                    if(lhs->nrm1<rhs->nrm1)
                    {
                        return -1;
                    }
                    else
                    {
                        if(rhs->nrm1<lhs->nrm1)
                        {
                            return 1;
                        }
                        else
                        {
                            return -comparison::lexicographic(*lhs,*rhs);
                        }

                    }

                }


                const unsigned nrm1;
                uvec          *next;
                uvec          *prev;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(uvec);

                inline static unsigned norm(const uarr &arr) noexcept
                {
                    unsigned sum = 0;
                    for(size_t i=arr.size();i>0;--i) sum += arr[i];
                    return sum;
                }
            };

            class conserving : public cxx_list_of<uvec>
            {
            public:
                const size_t dim;
                uarr         arr;

                inline  conserving(const size_t d) : dim(d), arr(dim) {}
                inline ~conserving() noexcept {}

                inline void operator()(const raven::qvector &cf)
                {
                    assert(dim==cf.size());
                    //std::cerr << cf << std::endl;
                    size_t         num = 0;
                    arr.ld(0);

                    for(size_t i=dim;i>0;--i)
                    {
                        const apq &q = cf[i];
                        switch( q.num.s )
                        {
                            case negative: return;
                            case __zero__: continue;
                            case positive:
                                ++num;
                                arr[i] = q.num.cast_to<unsigned>();
                                break;
                        }
                    }

                    if(num<2) return;

                    for(const uvec *v=head;v;v=v->next)
                    {
                        if( (*v) == arr ) return;
                    }

                    //std::cerr << "arr=" << arr << std::endl;
                    push_back( new uvec(arr) );
                }

                inline void sort()
                {
                    merge_list_of<uvec>::sort(*this, uvec::compare);
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(conserving);
            };

            


        }


        static inline
        size_t compress_ortho(matrix<int>       &Q0,
                              const matrix<int> &Q)
        {
            const size_t       nr = Q.rows; assert(Q.rows==Q.cols);
            cxx_series<size_t> ir(nr);
            for(size_t i=1;i<=nr;++i)
            {
                const readable<int> &Qi = Q[i];
                if( raven::qselect::count_valid(Qi) <=  0 )  continue;

                bool original = true;
                for(size_t j=ir.size();j>0;--j)
                {
                    if( alga::colinear(Qi,Q[ir[j]]))
                    {
                        original = false;
                        break;
                    }
                }

                if(original) ir << i;

            }

            const size_t nq = ir.size();
            if(nq<=0)
            {
                return 0;
            }
            else
            {
                Q0.make(nq,nr);
                for(size_t i=1;i<=nq;++i)
                {
                    math::iota::load(Q0[i],Q[ir[i]]);
                }
                return alga::rank(Q0);
            }

        }

        void domain:: build_conserved(const xmlog       &xml)
        {

            YACK_XMLSUB(xml,"build_conserved");

            //------------------------------------------------------------------
            //
            // extracting defined/original
            //
            //------------------------------------------------------------------
            eq_list hub;
            for(const eq_node *en=defined.head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                if(eq.indx[sub_level]>N) break;
                hub << eq;
            }

            const size_t nh = hub.size;
            if(nh<=0)
            {
                YACK_XMLOG(xml, "no defined equilibrium");
                return;
            }

            //------------------------------------------------------------------
            //
            // computing reduced topology matrix
            //
            //------------------------------------------------------------------
            matrix<int> P(nh,M);

            // loading topology
            {
                size_t i = 1;
                for(const eq_node *en=hub.head;en;en=en->next,++i)
                {
                    (***en).fill(P[i],sub_level);
                }
            }

            // reducing topology
            for(const sp_node *sn=endless.head;sn;sn=sn->next)
            {
                const species &sp = ***sn;
                const size_t   sj = sp.indx[sub_level];
                for(size_t i=nh;i>0;--i) P[i][sj] = 0;
            }
            const size_t p = alga::rank(P);
            if(p!=nh) throw imported::exception(clid,"invalid defined equilibri[um|a] rank!!");

            //------------------------------------------------------------------
            //
            // compute ortho-space
            //
            //------------------------------------------------------------------
            const size_t q = M-p;
            matrix<int>  Q(M,M);
            if(!math::ortho_family::build(Q,P,true)) throw imported::exception(clid,"couldn't compute _|_");
            //YACK_XMLOG(xml, "Q    = " << Q); assert( q == alga::rank(Q) );

            //------------------------------------------------------------------
            // compress ortho-space
            //------------------------------------------------------------------
            matrix<int> Q0;
            if( compress_ortho(Q0,Q) != q ) throw imported::exception(clid,"invalid ortho-compression");
            //YACK_XMLOG(xml, "Q0   = " << Q0);

            //------------------------------------------------------------------
            //
            // computing conserved vectors by RAVEn
            //
            //------------------------------------------------------------------
            conserving      cv(M);
            {
                raven::qbranch b;
                b.batch(Q0,q,cv);
            }
            cv.sort();

            //------------------------------------------------------------------
            //
            // creating conservations
            //
            //------------------------------------------------------------------
            unsigned k=0;
            for(const uvec *v=cv.head;v;v=v->next)
            {
                const readable<unsigned> &cof = *v; assert( raven::qselect::count_valid(cof) >= 2);
                conservation             *law = coerce(laws).push_back( new conservation() );
                for(const sp_node *sn=live.head;sn;sn=sn->next)
                {
                    const species &sp = ***sn;
                    const size_t   sj = sp.indx[sub_level];
                    const unsigned nu = cof[sj];
                    if(nu!=0)
                    {
                        law->push_back( new actor(sp,nu) );
                    }
                }
                YACK_XMLOG(xml,"V" << ++k << " = " << cof << " => 0=d" << *law);
            }

            //------------------------------------------------------------------
            //
            // creating groups
            //
            //------------------------------------------------------------------
            conserved_group &groups = coerce(clog);
            for(const conservation *law=laws.head;law;law=law->next)
            {
                for(conserved *lhs=groups.head;lhs;lhs=lhs->next)
                {
                    
                }
            }




        }
        
    }

}


