
//! \file

#ifndef YACK_IOS_FMT_BIN_INCLUDED
#define YACK_IOS_FMT_BIN_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! display tight binary
        //
        //______________________________________________________________________
        class binary
        {
        public:
            //! setup
            template <typename T>
            inline binary(const T x) throw() :
            qword( uint64_t(x)  ),
            nbits( width(qword) )
            {
            }


            binary(const binary &_) throw(); //!< copy
            ~binary() throw();               //!< cleanup

            //! display
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const binary &b)
            {
                char tab[64];
                size_t n  = b.load(tab);
                while(n-- > 0) os << tab[n];
                return os;
            }

        private:
            YACK_DISABLE_ASSIGN(binary);
            const uint64_t qword;
            const size_t   nbits;
            static size_t  width(const uint64_t) throw();
            size_t         load(char tab[]) const throw();
        };
    }

}
#endif
