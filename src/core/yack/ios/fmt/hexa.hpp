//! \file

#ifndef YACK_IOS_FMT_HEXA_INCLUDED
#define YACK_IOS_FMT_HEXA_INCLUDED 1

#include "yack/type/ints.hpp"

namespace yack
{

    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! display tight hexa
        //
        //______________________________________________________________________
        class hexa
        {
        public:
            //! setup
            template <typename T>
            inline hexa(const T x) throw() :
            qword( uint64_t(x) ),
            count(width(qword))
            {
            }

            hexa(const hexa &_) throw(); //!< copy
            ~hexa() throw();             //!< cleanup

            //! load textual data
            template <typename T>
            static size_t text(char tab[], const T x) throw()
            {
                const hexa _(x);
                return _.load(tab);
            }

            //! display
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const hexa &x)
            {
                char tab[16];
                size_t n = x.load(tab); assert(n>0);
                while(n-- > 0) os << tab[n];
                return os;
            }

        private:
            YACK_DISABLE_ASSIGN(hexa);
            const uint64_t qword;
            const size_t   count;
            static size_t  width(const uint64_t) throw();
            //! load chars in tab[0..15], return number of chars
            size_t load(char tab[]) const throw();


        };


    }
}


#endif

