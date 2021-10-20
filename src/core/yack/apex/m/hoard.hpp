//! \file

#ifndef YACK_APEX_HOARD_INCLUDED
#define YACK_APEX_HOARD_INCLUDED 1


#include "yack/arith/ilog2.hpp"
#include "yack/arith/base2.hpp"
#include "yack/data/core-pool.hpp"
#include "yack/arith/align.hpp"

namespace yack
{


    namespace apex
    {
        struct piece
        {
            piece   *next;
            piece   *prev;
        };

        class hoard
        {
        public:
            typedef core_pool_of<piece> repository;

            static const size_t min_block_size = sizeof(piece);
            static const size_t min_block_exp2 = ilog2<min_block_size>::value;
            static const size_t max_block_size = base2<size_t>::max_power_of_two;
            static const size_t max_block_exp2 = base2<size_t>::max_shift;
            static const size_t repositories   = 1+max_block_exp2-min_block_exp2;
            static const size_t repo_raw_bytes = repositories*sizeof(repository);
            static const size_t repo_bytes = YACK_ALIGN_TO(void *,repo_raw_bytes);
            static const size_t repo_words = repo_bytes/sizeof(void*);

            hoard() throw();
            ~hoard() throw();

            void *acquire_unlocked(size_t       &block_exp2);
            void  release_unlocked(void *block_addr, const size_t  block_exp2) throw();
            void  gc()      throw();
            void  display() const;

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hoard);
            repository *repo;
            void       *impl[repo_words];
        };





    }

}

#endif

