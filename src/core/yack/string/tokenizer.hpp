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
            inline virtual ~tokenizer() throw() { curr=0;last=0;size=0;indx=0; }

            //! setup from sub-string
            inline explicit tokenizer(const T *entry, const size_t count) throw() :
            curr(entry), last(curr+count), size(0), indx(0)
            {
            }

            //! setup from string
            inline explicit tokenizer(const string<T> &str) throw() :
            curr( str() ), last(curr+str.size()), size(0), indx(0)
            {
            }

            //! the main find function
            template <typename FUNCTION>
            bool find(FUNCTION &is_sep) throw()
            {
                curr += size;
            SKIP:
                if(curr>=last) return false;
                if(is_sep(*curr)) { ++curr; goto SKIP; }

                const T *scan = curr;
            SCAN:
                if(++scan>=last) goto DONE;
                if(!is_sep(*scan)) goto SCAN;

            DONE:
                size=scan-curr;
                ++indx;
                return true;
            }

            inline const T *token() const throw() { return curr; } //!< current token position
            inline size_t   units() const throw() { return size; } //!< current token units
            inline size_t   count() const throw() { return indx; } //!< current token count

            //! split source into words
            template <typename SEQUENCE, typename FUNCTION> static inline
            size_t split(SEQUENCE        &words,
                         FUNCTION        &is_separator,
                         const string<T> &source)
            {
                tokenizer tkn(source);
                return tkn.fill(words,is_separator);
            }

            //! split using one char
            template <typename SEQUENCE> static inline
            size_t split_with(const char C, SEQUENCE &words, const string<T> &source)
            {
                is_separator_t is_separator = { C };
                return split(words,is_separator,source);
            }
            
        private:
            const T     *curr; //!< current position
            const T     *last; //!< first invalid char
            size_t       size; //!< current token chars
            size_t       indx; //!< current index

            YACK_DISABLE_COPY_AND_ASSIGN(tokenizer);
            struct is_separator_t
            {
                int tgt;
                inline bool operator()(const int src) const throw() {
                    return src==tgt;
                }
            };

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


    //__________________________________________________________________________
    //
    //! base class
    //__________________________________________________________________________
    typedef kernel::tokenizer<char> tokenizer_;

    //__________________________________________________________________________
    //
    //
    //! default tokenizer
    //
    //__________________________________________________________________________
    class tokenizer : public tokenizer_
    {
    public:
        virtual ~tokenizer()              throw(); //!< cleanup
        explicit tokenizer(const string&) throw(); //!< setup
        explicit tokenizer(const char  *) throw(); //!< setup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(tokenizer);
    };

}


#endif
