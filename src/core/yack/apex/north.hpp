
//! \file

#ifndef YACK_APEX_NORTH_INCLUDED
#define YACK_APEX_NORTH_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/sequence/thin-array.hpp"

namespace yack
{

    namespace north
    {
        template <typename T> struct classify;

        template < > struct classify<apq>
        {
            typedef apq type;
            typedef apn l2_type;
        };

        template < > struct classify<int64_t>
        {
            typedef int64_t  type;
            typedef uint64_t l2_type;
        };

        template <typename T> class qvector : public readable<T>
        {
        public:
            typedef typename classify<T>::l2_type    l2_type;    //!< alias
            typedef typename readable<T>::type       type;       //!< alias
            typedef typename readable<T>::const_type const_type; //!< alias

            inline explicit qvector(type *ptr, const size_t num) :
            cf(ptr), sz(num), n2(0)
            {
                assert( yack_good(cf,sz) );
                --cf;
            }

            inline virtual ~qvector() throw() {}

            inline virtual size_t size() const throw() { return sz; }

            inline virtual const_type & operator[](size_t indx) const throw()
            {
                assert(indx>=1); assert(indx<=sz); return cf[indx];
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qvector);
            type        *cf;
            const size_t sz;

        public:
            const type n2;
        };






    }


}

#endif

