
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

            template <typename T> inline
            void operator()(const matrix<T> &mu,
                            const size_t     rk)
            {
                prune();
                try {
                    const size_t      nr = mu.rows;
                    cxx_array<size_t> id(nr); id.ld_incr(1);
                    for(size_t i=nr;i>0;--i)
                    {
                        rolling::down(id);
                        qlist.push_back( new qfamily(id,mu,rk,io) );
                    }

                    coerce(depth) = 1;
                }
                catch(...) { prune(); throw; }
            }

            const size_t depth;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qbranch);
            cxx_list_of<qfamily> qlist;

        public:
            qFund io;
        };

    }

}

#endif

