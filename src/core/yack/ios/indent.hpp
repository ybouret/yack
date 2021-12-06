//! \file

#ifndef YACK_IOS_INDENT_INCLUDED
#define YACK_IOS_INDENT_INCLUDED 1

#include "yack/setup.hpp"
#include <iosfwd>

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! indentation
        //
        //______________________________________________________________________
        class indent
        {
        public:
            indent(const size_t sz, const char sp = ' ') throw(); //!< setup
            indent(const indent &)                       throw(); //!< copy
            virtual ~indent()                            throw(); //!< cleanup

            //! generic output
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const indent &i)
            {
                const char sp = i.c;
                for(size_t ii=i.n;ii>0;--ii) os << sp;
                return os;
            }

        private:
            YACK_DISABLE_ASSIGN(indent);
            const size_t n;
            const char   c;
        };

        //______________________________________________________________________
        //
        //
        //! extended indentation
        //
        //______________________________________________________________________
        template <const size_t N>
        class xindent : public indent
        {
        public:
            inline xindent(const size_t sz, const char sp = ' ') throw() : indent(sz*N,sp) {} //!< setup
            inline xindent(const xindent &_) throw() : indent(_) {} //!< copy
            inline virtual ~xindent() throw() {} //!< cleanup

        private:
            YACK_DISABLE_ASSIGN(xindent);
        };


    }
}

#endif
