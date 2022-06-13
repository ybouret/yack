//! \file

#ifndef YACK_SORT_NWSWAPS_INCLUDED
#define YACK_SORT_NWSWAPS_INCLUDED

#include "yack/container/writable.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{
    namespace nwsrt
    {

        class swaps_
        {
        public:
            virtual ~swaps_() throw();

            const char * const name;
            const size_t       size;


        protected:
            explicit swaps_(const char  *sid,
                            const size_t dim) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(swaps_);


        };

        template <const size_t SIZE>
        class swaps : public swaps_
        {
        public:
            const size_t       work;

            inline virtual ~swaps() throw() {}

        protected:
            inline explicit swaps(const char   *sid,
                                  const size_t  num) throw() :
            swaps_(sid,SIZE),
            work(num)
            {
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(swaps);
        };

        template < const size_t SIZE, const size_t WORK>
        class pairs : public swaps<SIZE>
        {
        public:
            inline virtual ~pairs() throw() {}

            template <typename ARRAY> inline
            void sort(ARRAY &arr) throw()
            {
                assert(SIZE<=arr.size());
                for(size_t i=0;i<WORK;++i)
                {
                    typename ARRAY::type &lhs = arr[ LHS[i] ];
                    typename ARRAY::type &rhs = arr[ RHS[i] ];
                    if(rhs<lhs)
                    {
                        out_of_reach::swap(&lhs,&rhs,sizeof(typename ARRAY::type));
                    }
                }
            }

        protected:
            inline explicit pairs(const char   *sid,
                                  const size_t *lhs,
                                  const size_t *rhs) throw() :
            swaps<SIZE>(sid,WORK),
            LHS(lhs),
            RHS(rhs)
            {
                assert(NULL!=LHS);
                assert(NULL!=RHS);
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pairs);
            const size_t * const LHS;
            const size_t * const RHS;
        };

    };
}

#endif

