//! \file

#ifndef YACK_STRING_TOKENIZER_INCLUDED
#define YACK_STRING_TOKENIZER_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{

    namespace kernel
    {
        //______________________________________________________________________
        //
        //
        //! tokenizer for strings
        //
        //______________________________________________________________________
        template <typename T>
        class tokenizer
        {
        public:
            //! cleanup
            inline virtual ~tokenizer() throw() { curr=0;last=0;size=0; }

            //! setup from sub-string
            inline explicit tokenizer(const T *entry, const size_t count) throw() :
            curr(entry), last(curr+count), size(0)
            {
            }

            //! setup from string
            inline explicit tokenizer(const string<T> &str) throw() :
            curr(&str[1]), last(curr+str.size()), size(0)
            {
            }

            //! the main find function
            template <typename FUNCTION>
            bool find(FUNCTION &is_separator) throw()
            {
                curr += size;
            SKIP:
                if(curr>=last) return false;
                if(is_separator(*curr)) { ++curr; goto SKIP; }

                const T *scan = curr;
            SCAN:
                if(++scan>=last) goto DONE;
                if(!is_separator(*scan)) goto SCAN;

            DONE:
                size=scan-curr;
                return true;
            }

            inline const T *token() const throw() { return curr; } //!< current token position
            inline size_t   units() const throw() { return size; } //!< current token units

            //! split source into words
            template <typename SEQUENCE, typename FUNCTION> static inline
            size_t split(SEQUENCE        &words,
                         FUNCTION        &is_separator,
                         const string<T> &source)
            {
                tokenizer tkn(source);
                return tkn.fill(words,is_separator);
            }


        private:
            const T *curr; //!< current position
            const T *last; //!< first invalid char
            size_t   size; //!< current token chars
            YACK_DISABLE_COPY_AND_ASSIGN(tokenizer);
            
            template <typename SEQUENCE, typename FUNCTION> inline
            size_t fill(SEQUENCE &words, FUNCTION &is_separator)
            {
                size_t res = 0;
                while(find(is_separator))
                {
                    const string<T> tmp(curr,size);
                    words.push_back(tmp);
                    ++res;
                }
                return res;
            }
        };

    }

    typedef kernel::tokenizer<char> tokenizer; //!< alias

}


#endif
