#include "yack/concurrent/loop/simd.hpp"
#include "yack/type/utils.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace concurrent
    {

        class simd::worker
        {
        public:
            ~worker() throw();
            worker(const size_t size, const size_t rank);

            const context ctx;

            static inline
            worker *zalloc( size_t &capa )
            {
                static memory::dyadic &mgr = memory::dyadic::instance();
                return static_cast<worker*>(mgr.acquire(capa,sizeof(worker)));
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(worker);
        };

        const char simd:: clid[] = "simd";




        simd:: simd(const size_t n) :
        loop(),
        sync(clid),
        cond(),
        nthr( max_of<size_t>(n,1) ),
        capa(nthr),
        team( worker::zalloc(capa) )
        {


            --team;
        }

        simd:: ~simd() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();

            ++team;
            mgr.withdraw(team,capa);
        }


        size_t simd:: size() const throw()
        {
            return nthr;
        }

        loop::const_type & simd:: operator[](const size_t indx) const throw()
        {
            assert(NULL!=team);
            assert(indx>0);
            assert(indx<=nthr);
            assert(team[indx].ctx.indx==indx);
            return team[indx].ctx;
        }

        lockable & simd::access() throw()
        {
            return sync;
        }

        const char * simd:: family() const throw()
        {
            return clid;
        }

    }

}
