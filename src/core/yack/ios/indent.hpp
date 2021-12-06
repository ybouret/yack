//! \file

#ifndef YACK_IOS_INDENT_INCLUDED
#define YACK_IOS_INDENT_INCLUDED 1

#include "yack/setup.hpp"
#include <iosfwd>

namespace yack
{
    namespace ios
    {
        class indent
        {
        public:
            indent(const size_t sz, const char sp = ' ') throw();
            indent(const indent &) throw();
            virtual ~indent() throw();

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

        template <const size_t N>
        class xindent : public indent
        {
        public:
            inline xindent(const size_t sz, const char sp = ' ') throw() : indent(sz*N,sp) {}
            inline xindent(const xindent &_) throw() : indent(_) {}
            inline virtual ~xindent() throw() {}

        private:
            YACK_DISABLE_ASSIGN(xindent);
        };


    }
}

#endif
