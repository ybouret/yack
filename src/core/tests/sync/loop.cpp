#include "yack/concurrent/loop/mono.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/utest/run.hpp"
#include "yack/utest/tmx.hpp"
#include "yack/concurrent/thread.hpp"
#include "yack/type/utils.hpp"

#include <cstdlib>

using namespace yack;

namespace
{

    static inline
    void compute(const concurrent::context &here,
                 void                      * ,
                 lockable                  &sync)
    {
        {
            YACK_LOCK(sync);
            std::cerr << "  (*) Computing with " << here << std::endl;
        }

    }

    class engine : public concurrent::runnable
    {
    public:
        const size_t length;
        double      *partial;

        explicit engine(const size_t len) throw() :
        length(len),
        partial(0)
        {
        }

        virtual ~engine() throw()
        {
            clean();
        }

        void prepare(const size_t para)
        {
            clean();
            partial = new double[para];
        }

        void clean() throw()
        {
            if(partial) { delete []partial; partial=0; }
        }

        double get(const size_t para) const throw()
        {
            double res = 0;
            for(size_t i=0;i<para;++i)
            {
                res += partial[i];
            }
            return res;
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(engine);
        virtual void run(const concurrent::context &here, lockable &sync) throw()
        {
            size_t size = length;
            size_t curr = 1;
            here.crop(size,curr);
            if(false)
            {
                YACK_LOCK(sync);
                std::cerr << "@" << here << " : " << curr << "+" << size << std::endl;
            }
            double sum = 0;
            while(size-- > 0)
            {
                sum += 1.0/squared(double(curr++));
            }
            partial[here.rank] = sum;
        }
    };


}

YACK_UTEST(sync_loop)
{
    uprng ran;

    concurrent::thread::verbose = true;
    int        threads = 1;
    if(argc>1) threads = atoi(argv[1]);

    if(false)
    {
        int load = 6;
        int init = 1;

        for(size_t size=1;size<=8;++size)
        {
            std::cerr << "size=" << size << std::endl;
            int total = 0;
            for(size_t rank=0;rank<size;++rank)
            {
                int todo = load;
                int from = init;
                concurrent::split::with(size,rank,todo,from);
                std::cerr << "\t" << size << "." << rank << " @" << from << "+" << todo << std::endl;
                total += todo;
            }
            YACK_ASSERT(total==load);
        }
    }

    
    concurrent::mono seq;
    concurrent::simd par(threads);

    {
        seq(compute,NULL);
        for(size_t i=0;i<16;++i)
        {
            par(compute,NULL);
        }
    }

    concurrent::thread::verbose = false;
    engine eng(0x1000000);

    eng.prepare( seq.size() );

    seq.run(eng);
    std::cerr << "seq: " << eng.get(seq.size()) << std::endl;
    std::cerr << "seq rate:" << std::endl;
    double seqRate = 0;
    YACK_TMX_RATE(seqRate,seq.run(eng),1.0);
    std::cerr << "\t" << seqRate  << std::endl;


    eng.prepare( par.size() );
    par.run(eng);
    std::cerr << "par: " << eng.get(par.size()) << std::endl;

    std::cerr << "par rate:" << std::endl;
    double parRate = 0;
    YACK_TMX_RATE(parRate,par.run(eng),1.0);
    std::cerr << "\t" << parRate << std::endl;

    std::cerr << "speed up=" << parRate/seqRate << std::endl;

    std::cerr << "efficiency: " << par.efficiency(seqRate,parRate) << "%" << std::endl;



}
YACK_UDONE()



