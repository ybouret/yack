
#include "yack/memory/sentry.hpp"
#include "yack/hashing/sha1.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include "yack/arith/align.hpp"

#include <cstring>
#include <iostream>
#include <cstdlib>

namespace yack
{
    namespace memory
    {

        namespace {
            typedef hashing::sha1 hasher;

            struct                mblock
            {
                const char  *file;
                int          line;
                const void  *addr;
                size_t       size;
            };

            static const size_t k_length = YACK_MEMALIGN(sizeof(sentry::key_t));
            static const size_t b_length = YACK_MEMALIGN(sizeof(mblock));
            static const size_t h_offset = k_length+b_length;

            static inline
            hasher &getHasher(void *wksp) throw()
            {
                return *static_cast<hasher *>(out_of_reach::shift(wksp,h_offset));
            }

            static inline
            sentry::key_t &getKey(void *wksp) throw()
            {
                return *static_cast<sentry::key_t *>(out_of_reach::address(wksp));
            }

            static inline
            mblock & getBlk(void *wksp) throw()
            {
                return *static_cast<mblock *>(out_of_reach::shift(wksp,k_length) );
            }


        }

        void sentry:: setup(const char *file, const int line, const void *block_addr, const size_t block_size) throw()
        {
            assert( yack_good(block_addr,block_size) );
            assert(NULL!=file);
            memset(wksp,0,sizeof(wksp));
            mblock &B = getBlk(wksp);
            hasher &H = getHasher(wksp);
            key_t  &K = getKey(wksp);

            assert(0==K);
            assert(0==B.file);
            assert(0==B.line);
            assert(0==B.addr);
            assert(0==B.size);

            new (&H) hasher();
            assert(0==K);
            assert(0==B.file);
            assert(0==B.line);
            assert(0==B.addr);
            assert(0==B.size);

            B.file = file;
            B.line = line;
            B.addr = block_addr;
            B.size = block_size;
            assert(0==K);

            H.block(&K,sizeof(K),block_addr,block_size);
            assert(block_addr == B.addr);
            assert(block_size == B.size);

            //std::cerr << B.file << ":" << B.line << ": ";
            //std::cerr << "K=" << K << std::endl;
        }

        sentry:: sentry(const char  *file,
                        const int    line,
                        const void  *block_addr,
                        const size_t block_size) throw():
        wksp()
        {
            setup(file,line,block_addr,block_size);
        }

        sentry:: ~sentry() throw()
        {
            const key_t   K    = getKey(wksp);
            const mblock &B    = getBlk(wksp);
            hasher       &H    = getHasher(wksp);
            const char   *file = B.file; assert(file);
            const int     line = B.line;
            key_t         S    = 0;

            H.block(&S,sizeof(S),B.addr,B.size);

            destruct( & getHasher(wksp) );
            memset(wksp,0,sizeof(wksp));

            if(K!=S)
            {
                std::cerr << file << ':' << line << ": modified memory!" << std::endl;
                exit(1);
            }

        }

        sentry::key_t sentry:: operator*() const throw()
        {
            return getKey(wksp);
        }

    }

}

