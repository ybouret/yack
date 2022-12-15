
//! \file

#ifndef YACK_RAVEN_QBRANCH_INCLUDED
#define YACK_RAVEN_QBRANCH_INCLUDED 1

#include "yack/raven/qfamily.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/roll.hpp"

namespace yack
{
    namespace raven
    {

        class qbranch
        {
        public:
            explicit qbranch();
            virtual ~qbranch() throw();

            void prune() throw();

            const list_of<qfamily> & operator*()  const throw();
            const list_of<qfamily> * operator->() const throw();
            friend std::ostream & operator<<(std::ostream &os, const qbranch &);

            //! initialize branch
            template <typename T, typename FUNC> inline
            void operator()(const matrix<T> &mu,
                            const size_t     rk,
                            FUNC            &ok)
            {
                prune();
                try {
                    const size_t      nr = mu.rows;
                    cxx_array<size_t> id(nr); id.ld_incr(1);
                    for(size_t i=1;i<=nr;++i)
                    {
                        rolling::down(id); assert(i==id[nr]);
                        if(!ok(mu[i])) continue;;
                        qlist.push_back( new qfamily(id,mu,rk,io) );
                        coerce(depth) = 1;
                    }
                }
                catch(...) { prune(); throw; }
            }

            template <typename T, typename PROC>
            inline size_t generate(const matrix<T> &mu,
                                   PROC            &cb)
            {
                {
                    qfamily::list_type target;
                    while(qlist.size)
                    {
                        const auto_ptr<qfamily> parents = qlist.pop_front();
                        qfamily::list_type      lineage;
                        parents->generate(lineage,mu);

                        for(const qfamily *f=lineage.head;f;f=f->next)
                            cb( (**f).last() );

                        qfamily::reduce(lineage);

                        // fusion
                        target.merge_back(lineage);
                    }
                    target.swap_with(qlist);
                }
                const size_t nf = qlist.size;
                if(nf)
                {
                    ++coerce(depth);
                    return nf;
                }
                else
                {
                    coerce(depth) = 0;
                    return 0;
                }
            }


            const size_t depth;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qbranch);
            qfamily::list_type qlist;

        public:
            qFund io;

        };

    }

}

#endif

