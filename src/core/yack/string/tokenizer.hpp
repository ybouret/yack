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
            inline virtual ~tokenizer() noexcept { curr=0;last=0;size=0;indx=0; }

            //! setup from sub-string
            inline explicit tokenizer(const T *entry, const size_t count) noexcept :
            curr(entry), last(curr+count), size(0), indx(0)
            {
            }

            //! setup from string
            inline explicit tokenizer(const string<T> &str) noexcept :
            curr( str() ), last(curr+str.size()), size(0), indx(0)
            {
            }

            //! the main find function
            template <typename FUNCTION>
            bool find(FUNCTION &is_sep) noexcept
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

            inline const T *token() const noexcept { return curr;      } //!< current token position
            inline size_t   units() const noexcept { return size;      } //!< current token units
            inline size_t   count() const noexcept { return indx;      } //!< current token count

            //! after current token
            inline const T *after() const noexcept
            {
                const T *next = curr+size;
                if(++next>=last) return last; else return next;
            }

            //! find with a given separator
            inline bool find_with(const char C) noexcept
            {
                is_separator_t is_separator = { C };
                return find(is_separator);
            }

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
                inline bool operator()(const int src) const noexcept {
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
        virtual ~tokenizer()              noexcept; //!< cleanup
        explicit tokenizer(const string&) noexcept; //!< setup
        explicit tokenizer(const char  *) noexcept; //!< setup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(tokenizer);
    };

}


#endif
