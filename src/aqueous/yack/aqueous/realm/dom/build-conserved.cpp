
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/raven/qselect.hpp"


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

            typedef cxx_array<unsigned>           uarray;
            typedef vector<uarray,memory::dyadic> uarrays;
            class conserved : public uarrays
            {
            public:
                const size_t dims;
                inline  conserved(const size_t d) noexcept :  uarrays(), dims(d) {}
                inline ~conserved() noexcept {}

                static inline unsigned sum(const uarray &arr) noexcept
                {
                    unsigned res = 0;
                    for(size_t i=arr.size();i>0;--i) res += arr[i];
                    return res;
                }

                static inline int compare(const uarray &lhs,
                                          const uarray &rhs) noexcept
                {
                    const unsigned l = sum(lhs);
                    const unsigned r = sum(rhs);
                    return comparison::increasing(l,r);
                }

                void sort() noexcept
                {
                    hsort(*this,compare);
                }

                inline void operator()(const raven::qvector &cf)
                {
                    assert(dims==cf.size());
                    std::cerr << cf << std::endl;
                    size_t num = 0;
                    uarray arr(dims);
                    for(size_t i=dims;i>0;--i)
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
                    for(size_t i=size();i>0;--i)
                    {
                        if( (*this)[i] == arr ) return;
                    }

                    std::cerr << "arr=" << arr << std::endl;
                    (*this) << arr;
                }

                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(conserved);
            };

        }

        void domain:: build_conserved(const xmlog &xml)
        {
            //YACK_XMLOG(xml,"build_conserved");
            if(M<=N) return ;

            // initialize
            const size_t q = M-N;   // rank of ortho
            matrix<int>  Q(M,M);    // ortho family

            // compute ortho-space
            if(!math::ortho_family::build(Q,Nu,true)) throw imported::exception(clid,"couldn't compute Nu _|_");
            YACK_XMLOG(xml, "Q    = " << Q); assert(q==alga::rank(Q));

            // compress ortho-space
            matrix<int> Q0;
            if( raven::qselect::compress(Q0,Q) != q ) throw imported::exception(clid,"invalid ortho-compression");
            YACK_XMLOG(xml, "Q0   = " << Q0); assert(q==alga::rank(Q));

            conserved      cv(M);
            {
                raven::qbranch b;
                b.batch(Q0,q,cv);
            }
            cv.sort();
            std::cerr << cv << std::endl;

        }
        
    }

}


