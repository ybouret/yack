//! \file

#ifndef YACK_IOS_FMT_HEXA_INCLUDED
#define YACK_IOS_FMT_HEXA_INCLUDED 1

#include "yack/type/ints.hpp"
#include "yack/ios/fwd.hpp"
#include <iosfwd>

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
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            template <typename T>
            inline hexa(const T x,const bool full=false) noexcept :
            qword( uint64_t(x)  ),
            count( full? sizeof(T) : width(qword) )
            {
            }

            hexa(const hexa &_) noexcept; //!< copy
            ~hexa() noexcept;             //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! load textual data
            template <typename T>
            static size_t text(char tab[], const T x) noexcept
            {
                const hexa _(x);
                return _.load(tab);
            }
            
            friend std::ostream & operator<<(std::ostream &, const hexa &x); //!< char-wise output
            friend ios::ostream & operator<<(ios::ostream &, const hexa &x); //!< frame output


        private:
            YACK_DISABLE_ASSIGN(hexa);
            const uint64_t qword;
            const size_t   count;
            static size_t  width(const uint64_t) noexcept;
            //! load chars in tab[0..15], return number of chars
            size_t load(char tab[]) const noexcept;


        };


    }
}


#endif

