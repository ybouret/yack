#ifndef YACK_STRING_INCLUDED
#define YACK_STRING_INCLUDED 1

#include "yack/string/utf8.hpp"

namespace yack
{

    //! string
    class string
    {
    public:
        explicit string();
        virtual ~string() throw();

    private:
        size_t  width;  //!< max count
        size_t  count;  //!< UTF-8
        utf8   *entry;  //!< entry[1..count]
        size_t  c_room; //!< c_room = 1 + 4 * count
        size_t  c_maxi; //!< c_maxi=c_room-1
        size_t  c_size; //!< c_size<=c_maxi
        char   *c_addr; //!< buffer
    };

}

#endif

