#include "yack/raven/qmatrix.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace raven
    {

        qmatrix:: ~qmatrix() throw()
        {

        }


        qmatrix:: qmatrix(const size_t sz,
                          const size_t rk) :
        qmetrics(sz),
        readable<qvector>(),
        maximum_rank(rk),
        current_rank(0),
        lib(),
        obj()
        {
            if(maximum_rank<2) throw imported::exception("raven::matrix","maximum_rank<2");

        }

        size_t qmatrix:: size() const throw() { return current_rank; }

        const qvector & qmatrix:: operator[](const size_t ivec) const throw()
        {
            assert(ivec>=1); assert(ivec<=maximum_rank);
            return row[ivec];
        }

    }

}

#include "yack/memory/embed.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace raven
    {

        void qmatrix:: initialize()
        {
            static memory::allocator &mgr = memory::dyadic::instance();

            qvector     *pqv = NULL;
            const size_t nqv = maximum_rank;

            apz         *pit = NULL;
            const size_t nit = dimension*maximum_rank + dimension;


            qvector     *pgs = NULL;
            const size_t ngs = dimension;

            memory::embed emb[] =
            {
                memory::embed(pqv,nqv),
                memory::embed(pit,nit),
                memory::embed(pgs,ngs)
            };

            lib.build(emb, sizeof(emb)/sizeof(emb[0]),mgr);


        }

    }

}
