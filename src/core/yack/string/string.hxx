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
        collection(),
        string_(0,sizeof(CH)),
        writable<CH>(),
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

        template <>
        void string<CH>:: xch(string &other) throw()
        {
            swap_with(other);
            cswap(item,other.item);
        }

        template <>
        string<CH>:: string(const string &other) :
        collection(),
        string_(other,sizeof(CH)),
        writable<CH>(),
        item( static_cast<CH*>(block)-1 )
        {
        }

        template <>
        string<CH> &  string<CH>:: operator=(const string &other)
        {
            string tmp(other);
            xch(tmp);
            return *this;
        }

    }
}
