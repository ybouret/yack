#include "yack/string/string.hpp"
#include "yack/type/out-of-reach.hpp"
#include <cstring>

namespace yack
{
    namespace kernel
    {

       
        
        template <>
        void string<CH>:: xch(string &other) throw()
        {
            swap_with(other);
            cswap(item,other.item);
        }

        template <>
        void  string<CH> :: clear() throw()
        {
            memset(block,0,chars*sizeof(CH));
            chars = 0;
        }

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
        string<CH>::  string(const size_t n, const as_capacity_t &) :
        collection(),
        string_(n,sizeof(CH)),
        writable<CH>(),
        item( static_cast<CH*>(block)-1 )
        {
        }



        template <>
        string<CH>:: string(const CH ch) :
        collection(),
        string_(1,sizeof(CH)),
        writable<CH>(),
        item( static_cast<CH*>(block)-1 )
        {
            item[chars=1] = ch;
        }

        template <>
        string<CH> & string<CH>:: operator=(const CH ch) throw()
        {
            clear();
            item[chars=1] = ch;
            return *this;
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
        string<CH> & string<CH>:: operator=(const string &other)
        {
            if(other.chars>width)
            {
                string<CH> tmp(other);
                xch(tmp);
            }
            else
            {
                const ptrdiff_t shift = ptrdiff_t(chars=other.chars) * sizeof(CH);
                memmove(block,other.block,shift);
                memset(out_of_reach::shift(block,shift),0,(width-chars)*sizeof(CH));
            }
            return *this;
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
        string<CH>   string<CH>::cat(const string<CH> &lhs, const string<CH> &rhs)
        {
            const size_t L = lhs.chars;
            const size_t R = rhs.chars;
            string<CH>   ans(L+R,as_capacity);

            size_t &i=ans.chars;
            for(size_t j=1;j<=L;++j)
            {
                ans.item[++i] = lhs[j];
            }
            for(size_t j=1;j<=R;++j)
            {
                ans.item[++i] = rhs[j];
            }
            return ans;
        }

        template <>
        string<CH> string<CH>::cat(const string<CH> &lhs, const CH rhs)
        {
            const size_t L = lhs.chars;
            string<CH>   ans(L+1,as_capacity);
            size_t      &i = ans.chars;
            for(size_t j=1;j<=L;++j)
            {
                ans.item[++i] = lhs[j];
            }
            ans.item[++i] = rhs;
            return ans;
        }

        template <>
        string<CH> string<CH>::cat(const CH lhs, const string<CH> &rhs)
        {
            const size_t R = rhs.chars;
            string<CH>   ans(R+1,as_capacity);
            size_t      &i = ans.chars;

            ans.item[++i] = lhs;
            for(size_t j=1;j<=R;++j)
            {
                ans.item[++i] = rhs[j];
            }
            return ans;
        }

        template <> string<CH> & string<CH>:: operator+=(const string<CH> &rhs)
        {
            const size_t L = chars;
            const size_t R = rhs.chars;
            if(L+R<=width)
            {
                size_t &i = chars;
                for(size_t j=1;j<=R;++j)
                {
                    item[++i] = rhs[j];
                }
            }
            else
            {
                string tmp = *this + rhs;
                xch(tmp);
            }
            return *this;
        }

        template <> string<CH> & string<CH>:: operator+=(const CH rhs)
        {
            const size_t L = chars;
            if(L+1<=width)
            {
                item[++chars] = rhs;
            }
            else
            {
                string tmp = *this + rhs;
                xch(tmp);
            }
            return *this;
        }


        template <> string<CH> & string<CH>:: skip(const size_t n) throw()
        {
            if(n>=chars)
            {
                clear();
            }
            else
            {
                chars -=n;
                memmove(block,out_of_reach::shift(block,ptrdiff_t(n) * sizeof(CH)),chars*sizeof(CH));
                memset(out_of_reach::shift(block,ptrdiff_t(chars)*sizeof(CH)),0,n*sizeof(CH));
            }
            return *this;
        }

        template <> string<CH> & string<CH>:: trim(const size_t n) throw()
        {
            if(n>=chars)
            {
                clear();
            }
            else
            {
                for(size_t i=n;i>0;--i)
                {
                    item[chars--] = 0;
                }
            }
            return *this;
        }

        
        template <>
        std::ostream & string<CH>:: display(std::ostream &os) const
        {
            for(size_t i=1;i<=chars;++i)
            {
                os << item[i];
            }
            return os;
        }
        
    }
}
