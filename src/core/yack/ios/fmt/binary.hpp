
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
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            template <typename T>
            inline binary(const T x, const bool full=false) noexcept :
            qword( uint64_t(x)  ),
            nbits( full ? (sizeof(T)*8) : width(qword) )
            {
            }

            binary(const binary &_) noexcept; //!< copy
            ~binary() noexcept;               //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const binary &x);  //!< char-wise output
            friend ios::ostream & operator<<(ios::ostream &, const binary &x);  //!< frame output

        private:
            YACK_DISABLE_ASSIGN(binary);
            const uint64_t qword;
            const size_t   nbits;
            static size_t  width(const uint64_t) noexcept;
            size_t         load(char tab[]) const noexcept;
        };
    }

}
#endif
