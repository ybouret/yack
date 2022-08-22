//! \file

#ifndef YACK_INFO_COMPRESSION_ALPHABET_INCLUDED
#define YACK_INFO_COMPRESSION_ALPHABET_INCLUDED 1

#include "yack/data/list/raw.hpp"
#include "yack/arith/align.hpp"

namespace yack
{
    namespace information
    {
        //______________________________________________________________________
        //
        //
        //! alphabets with codes and frequency
        //
        //______________________________________________________________________
        class alphabet
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const size_t    bytes = 256;            //!< bytes
            static const size_t    cntrl = 2;              //!< number of controls
            static const size_t    codes = bytes+cntrl;    //!< number of codes
            typedef      uint32_t  code_type;              //!< encoding
            typedef      uint32_t  freq_type;              //!< frequency
            static const code_type NYT = bytes;            //!< Not Yet Transmitted code
            static const code_type END = NYT+1;            //!< END of stream marker

            //__________________________________________________________________
            //
            //! internal node for char+info
            //__________________________________________________________________
            struct node_t
            {
                code_type  code; //!< initial code: 0..codes-1
                freq_type  freq; //!< frequency
                code_type  info; //!< binary representation
                code_type  bits; //!< bits for info
                node_t    *next; //!< for list
                node_t    *prev; //!< for list
            };

            static const size_t          space = codes * sizeof(node_t); //!< required bytes for nodes
            typedef raw_list_of<node_t>  list_t;                         //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit alphabet() throw(); //!< initialize
            virtual ~alphabet() throw(); //!< cleanup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const node_t &operator[](const uint8_t ch) const throw(); //!< access

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
