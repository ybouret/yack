#include "yack/apex/north/qbranch.hpp"
#include "yack/apex/north/qselect.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/roll.hpp"
#include "yack/sequence/list.hpp"
#include "yack/apex/kernel.hpp"

using namespace yack;

namespace {

    static inline
    void display_vec(const readable<apz> &q )
    {
        std::cerr << "\t" << q << std::endl;
    }

    class vecstore : public list< vector<apz> >
    {
    public:
        typedef vector<apz> vtype;

        explicit vecstore() throw() {}
        virtual ~vecstore() throw() {}

        void operator()(const readable<apz> &q)
        {
            for(iterator it=begin();it!=end();++it)
            {
                if( *it == q) return;
            }

            const vtype tmp(q,transmogrify);
            push_back(tmp);
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(vecstore);
    };

    static inline void create_topo(matrix<int> &nu, randomized::bits &ran)
    {
        for(size_t i=1;i<=nu.rows;++i)
        {
            for(size_t j=1;j<=nu.cols;++j)
            {
                const double p = ran.to<double>();
                if(p<0.0) {
                    nu[i][j] = 0;
                }
                else
                {
                    nu[i][j] = static_cast<int>( ran.in(-3,3) );
                }
            }
        }
    }

    static inline void test_qb(north::qbranch &source,
                               const size_t    eqs,
                               const size_t    spc,
                               randomized::bits &ran)
    {

        vecstore repo;
        assert(0==source->size);
        std::cerr << "#eqs=" << eqs << " | #spc=" << spc << std::endl;
        {
            matrix<int> nu(eqs,spc);
            do
            {
                create_topo(nu,ran);
            } while( apk::rank_of(nu) < eqs );

            std::cerr << "\tnu=" << nu << std::endl;
            matrix<int>  mu;
            const size_t rk = north::qselect::compress(mu,nu);
            std::cerr << "\tmu=" << mu << " #rank=" << rk << std::endl;

            const size_t     m = mu.rows;
            vector<size_t>   jx(m); jx.ld_incr(1);

            for(size_t j=1;j<=m;++j)
            {
                rolling::down(jx); assert(jx[m]==j);
                source.boot(jx,mu);
            }

            do{

                std::cerr << "\t\tdepth=" << std::setw(6) << source.depth << ": #" << std::setw(6) << source->size << std::endl;
                //std::cerr << "\t\t" << source << std::endl;
                if(source.depth>1)
                {
                    const size_t old_count = repo.size();
                    source.for_each(repo);
                    const size_t new_count = repo.size();
                    std::cerr << "\t\t\trepo: " << std::setw(6) << old_count << " -> " << std::setw(6) << new_count << " | +" << std::setw(6) << new_count - old_count << std::endl;
                }
                if(rk==source.depth) {
                    //std::cerr << "\t\tfinal: " << source << std::endl;
                }
            } while( source.next(mu) );
        }

        std::cerr << "\t#io   = " << source.cache->size << std::endl;
        source.cache->release();
        std::cerr << "\t#repo = " << repo.size() << std::endl;
        for(size_t i=1;i<=repo.size();++i)
        {
            //std::cerr << "\t\t" << repo[i] << std::endl;
        }
        (std::cerr << "\tcleaning source..." << std::endl).flush();
        source.prune();
        std::cerr << "\t...clean!" << std::endl;
    }

}

YACK_UTEST(apex_north_family)
{
    YACK_SIZEOF(north::qfamily);


    matrix<int> vec(4,3);
    vec[1][1] = 2;  vec[1][2] = 2;  vec[1][3] = 2;
    vec[2][1] = 0;  vec[2][2] = 1;  vec[2][3] = 0;
    vec[3][1] = 1;  vec[3][2] = 1;  vec[3][3] = 0;
    vec[4][1] = 0;  vec[4][2] = 0;  vec[4][3] = 1;

    vector<size_t>   rindx(4);
    north::qidx_bptr idxIO = new north::qidx_bank();
    rindx.ld_incr(1);
    std::cerr << "rindx=" << rindx << std::endl;
    std::cerr << "vbase=" << vec << std::endl;


    north::qbranch source;

    std::cerr << "Loading source" << std::endl;
    for(size_t i=1;i<=vec.rows;++i)
    {
        rolling::down(rindx);
        source.boot(rindx,vec);
    }

    std::cerr << source << std::endl;


    while( source.next(vec) )
    {
        std::cerr << source << std::endl;
        source.for_each(display_vec);
    }

    
    randomized::rand_ ran;
    for(size_t eqs=2; eqs<=5; ++eqs)
    {
        for(size_t spc=eqs;spc<=eqs+2;++spc)
        {
            test_qb(source,eqs,spc,ran);
        }
    }
    YACK_SIZEOF(north::qfamily);

}
YACK_UDONE()

