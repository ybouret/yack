//! \file

#ifndef YACK_RAVEN_QFAMILY_INCLUDED
#define YACK_RAVEN_QFAMILY_INCLUDED 1

#include "yack/raven/qmatrix.hpp"
#include "yack/data/small/set.hpp"
#include "yack/ptr/clone.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace raven
    {

        class qfamily : public object
        {
        public:
            virtual ~qfamily() throw();

            template <typename T> inline
            explicit qfamily(readable<size_t> &id,
                             const matrix<T>  &mu,
                             const size_t      rk) :
            object(),
            qbase( new qmatrix(mu.cols,rk) )
            {
                const size_t nr = mu.rows;  assert(nr>=2);
                const size_t ir = id[nr];   assert(ir>=1); assert(ir<=mu.rows);
                qmatrix     &Q  = *qbase;
                if(!Q(mu[ir])) throw_singular_matrix(ir);
                std::cerr << "Q=" << Q << std::endl;
            }


            clone_ptr<qmatrix> qbase;



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qfamily);
            void throw_singular_matrix(const size_t ir) const;
        };

    }

}

#endif

