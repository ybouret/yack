//! \file

#ifndef YACK_SORT_NWSWAPS_INCLUDED
#define YACK_SORT_NWSWAPS_INCLUDED

#include "yack/setup.hpp"

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

