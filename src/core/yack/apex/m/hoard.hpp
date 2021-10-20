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


        //______________________________________________________________________
        //
        //
        //! memory hoard for apex
        //
        //______________________________________________________________________
        class hoard
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! alias for memory blocks
            struct piece
            {
                piece   *next; //!< for repository
                piece   *prev; //!< optional for list ops
            };
            typedef core_pool_of<piece> repository; //!< alias

            static const size_t min_block_size = sizeof(piece);                    //!< minimal block size
            static const size_t min_block_exp2 = ilog2<min_block_size>::value;     //!< min_block_size = 2^min_block_exp2
            static const size_t max_block_size = base2<size_t>::max_power_of_two;  //!< maximal block size
            static const size_t max_block_exp2 = base2<size_t>::max_shift;         //!< max_block_size = 2^max_block_exp2
            static const size_t repositories   = 1+max_block_exp2-min_block_exp2;  //!< all possible block sizes
            static const size_t repo_raw_bytes = repositories*sizeof(repository);  //!< alias
            static const size_t repo_bytes = YACK_ALIGN_TO(void *,repo_raw_bytes); //!< alias
            static const size_t repo_words = repo_bytes/sizeof(void*);             //!< alias

            hoard()  throw(); //!< setup
            ~hoard() throw(); //!< cleanup

            //! acquire, adjust block_exp2
            /**
             warning, memory may be dirty!!!
             */
            void *acquire_unlocked(size_t &block_exp2);
            void  release_unlocked(void *block_addr, const size_t  block_exp2) throw(); //!< release a previously acquire block
            void  gc()      throw(); //!< garbage collection in memory dyadic
            void  display() const;   //!< info

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hoard);
            repository *repo;
            void       *impl[repo_words];
        };





    }

}

#endif

