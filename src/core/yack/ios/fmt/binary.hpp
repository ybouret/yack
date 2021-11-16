
//! \file

#ifndef YACK_IOS_FMT_BIN_INCLUDED
#define YACK_IOS_FMT_BIN_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/ios/fwd.hpp"
#include <iosfwd>
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

            friend std::ostream & operator<<(std::ostream &, const binary &x);
            friend ios::ostream & operator<<(ios::ostream &, const binary &x);

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
