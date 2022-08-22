//! \file

#ifndef YACK_INFO_COMPRESSION_ALPHABET_INCLUDED
#define YACK_INFO_COMPRESSION_ALPHABET_INCLUDED 1

#include "yack/data/list/raw.hpp"
#include "yack/arith/align.hpp"

namespace yack
{
    namespace information
    {

        class alphabet
        {
        public:
            static const size_t   bytes = 256;
            static const size_t   cntrl = 2;
            static const size_t   codes = bytes+cntrl;
            typedef      uint32_t code_type;
            typedef      uint32_t freq_type;
            static const uint32_t NYT = bytes;
            static const uint32_t END = NYT+1;
            
            struct node_t
            {
                code_type  code;
                freq_type  freq;
                code_type  info;
                code_type  bits;
                node_t    *next;
                node_t    *prev;
            };
            static const size_t          space = codes * sizeof(node_t);
            typedef raw_list_of<node_t> list_t;

            explicit alphabet() throw();
            virtual ~alphabet() throw();
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(alphabet);
            list_t  chars;
            node_t *nodes;
            node_t *nyt;
            node_t *end;
            void   *words[ YACK_WORDS_GEQ(space) ];
        };

    }
}

#endif
