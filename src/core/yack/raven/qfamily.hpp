//! \file

#ifndef YACK_RAVEN_QFAMILY_INCLUDED
#define YACK_RAVEN_QFAMILY_INCLUDED 1

#include "yack/raven/qmatrix.hpp"
#include "yack/data/small/set.hpp"
#include "yack/ptr/clone.hpp"
#include "yack/container/matrix.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace raven
    {

        typedef small_set<size_t>  qList;
        typedef small_bank<size_t> qBank;
        typedef qBank::pointer     qFund;

        class qfamily : public object
        {
        public:
            typedef cxx_list_of<qfamily> list_type;

            virtual ~qfamily() throw();

            template <typename T> inline
            explicit qfamily(readable<size_t> &id,
                             const matrix<T>  &mu,
                             const size_t      rk,
                             const qFund      &io) :
            object(),
            qbase( new qmatrix(mu.cols,rk) ),
            basis(io),
            ready(io),
            next(0),
            prev(0)
            {
                const size_t nr = mu.rows;  assert(nr>=2);
                const size_t ir = id[nr];   assert(ir>=1); assert(ir<=mu.rows);
                qmatrix     &Q  = *qbase;
                if(!Q(mu[ir])) throw_singular_matrix(ir);
                basis << ir;
                for(size_t i=1;i<nr;++i) ready << id[i];
            }

            qfamily(const qfamily &);


            friend std::ostream & operator<<(std::ostream &, const qfamily &);

            template <typename T> inline
            void generate(list_of<qfamily> &lineage,
                          const matrix<T>  &mu) const
            {
                assert(ready->size+basis->size==mu.rows);
                switch(qbase->active_state)
                {
                    case fully_grown: return;
                    case almost_done:
                        std::cerr << "Almost Done!" << std::endl;
                        final_generation(lineage,mu);
                        exit(0);
                        return;

                    case in_progress:
                        std::cerr << "In Progress!" << std::endl;
                        exit(0);
                        break;
                }
            }



            clone_ptr<qmatrix> qbase;
            qList              basis;
            qList              ready;
            qfamily           *next;
            qfamily           *prev;

        private:
            YACK_DISABLE_ASSIGN(qfamily);
            void throw_singular_matrix(const size_t ir) const;

            template <typename T> inline
            void final_generation(list_of<qfamily> &lineage,
                                  const matrix<T>  &mu) const
            {
                std::cerr << "Generating Final Generation" << std::endl;
                auto_ptr<qfamily> children = new qfamily(*this);
                assert(almost_done==children->qbase->active_state);
                std::cerr << children << std::endl;

            }



        };

    }

}

#endif

