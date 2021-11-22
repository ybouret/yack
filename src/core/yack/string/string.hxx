#include "yack/string/string.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{
    namespace kernel
    {
        template <>
        string<CH>:: ~string() throw()
        {
            item=0;
        }

        template <>
        string<CH>:: string() :
        string_(0,sizeof(CH)),
        item( static_cast<CH*>(block)-1 )
        {
        }

        template <>
        size_t string<CH>:: measure() const throw()
        {
            return chars * sizeof(CH);
        }

        template <>
        size_t string<CH>:: size() const throw()
        {
            return chars;
        }

        template <>
        string<CH>::const_type &string<CH>::operator[](const size_t indx) const throw()
        {
            assert(indx>=1);
            assert(indx<=chars+1);
            return item[indx];
        }

        template <>
        string<CH>::type &string<CH>::operator[](const size_t indx) throw()
        {
            assert(indx>=1);
            assert(indx<=chars);
            return item[indx];
        }


    }
}
