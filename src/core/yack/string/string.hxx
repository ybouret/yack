#include "yack/string/string.hpp"
#include "yack/type/out-of-reach.hpp"
#include <cstring>

namespace yack
{
    namespace kernel
    {
        template <>
        const string<CH> & string<CH>::key() const noexcept { return *this; }


        template <>
        const CH * string<CH>:: operator()(void) const noexcept
        {
            return static_cast<const CH*>(block);
        }

       
        
        template <>
        void string<CH>:: xch(string &other) noexcept
        {
            swap_with(other);
            cswap(item,other.item);
        }

        template <>
        void  string<CH> :: clear() noexcept
        {
            memset(block,0,chars*sizeof(CH));
            chars = 0;
        }

        template <>
        string<CH>:: ~string() noexcept
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
        string<CH>::  string(const size_t n, const as_capacity_t &, const bool filled) :
        collection(),
        string_(n,sizeof(CH)),
        writable<CH>(),
        item( static_cast<CH*>(block)-1 )
        {
            if(filled) chars=n;
        }



        template <>
        string<CH>:: string(const CH *entry, const size_t count) :
        collection(),
        string_(count,sizeof(CH)),
        writable<CH>(),
        item( static_cast<CH*>(block)-1 )
        {
            chars = count;
            if(entry) memcpy(block,entry,count*sizeof(CH));
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
        string<CH> & string<CH>:: operator=(const CH ch) noexcept
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
        size_t string<CH>:: measure() const noexcept
        {
            return chars * sizeof(CH);
        }

        template <>
        size_t string<CH>:: size() const noexcept
        {
            return chars;
        }

        template <>
        string<CH>::const_type &string<CH>::operator[](const size_t indx) const noexcept
        {
            assert(indx>=1);
            assert(indx<=chars+1);
            return item[indx];
        }

        template <>
        string<CH>::type &string<CH>::operator[](const size_t indx) noexcept
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


        template <> string<CH> & string<CH>:: skip(const size_t n) noexcept
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

        template <> string<CH> & string<CH>:: trim(const size_t n) noexcept
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
        
        template <>
        size_t string<CH>:: length_of(const CH *msg) noexcept
        {
            if(msg)
            {
                size_t cnt = 0;
                while( 0 != *(msg++) ) ++cnt;
                return cnt;
            }
            else
            {
                return 0;
            }
        }
        
        template <>
        bool string<CH>:: eq(const CH    *l,
                             const size_t nl,
                             const CH    *r,
                             const size_t nr) noexcept
        {
            assert(!(NULL==l&&nl>0));
            assert(!(NULL==r&&nr>0));
            
            if(nl!=nr)
            {
                return false;
            }
            else
            {
                for(size_t i=nl;i>0;--i)
                {
                    if( *(l++) != *(r++) ) return false;
                }
                return true;
            }
        }
        
        template <>
        bool string<CH>:: neq(const CH    *l,
                              const size_t nl,
                              const CH    *r,
                              const size_t nr) noexcept
        {
            assert(!(NULL==l&&nl>0));
            assert(!(NULL==r&&nr>0));
            
            if(nl!=nr)
            {
                return true;
            }
            else
            {
                for(size_t i=nl;i>0;--i)
                {
                    if( *(l++) != *(r++) ) return true;
                }
                return false; // are equal!
            }
        }

        template <> int  string<CH>:: cmp(const CH *l, const size_t nl,
                                          const CH *r, const size_t nr) noexcept
        {
            assert(!(NULL==l&&nl>0));
            assert(!(NULL==r&&nr>0));
            const CH *small = l;
            const CH *great = r;
            size_t    ns    = nl;
            size_t    ng    = nr;
            bool      exch  = false;
            if(ng<ns)
            {
                cswap(small,great);
                cswap(ns,ng);
                exch = true;
            }

            // compare small parts
            for(size_t i=0;i<ns;++i)
            {
                const CH s = *(small++);
                const CH g = *(great++);
                if(s<g)
                {
                    return exch ? 1:-1;
                }
                else
                {
                    if(g<s)
                    {
                        return exch ? -1:1;
                    }
                    else
                    {
                        assert(g==s);
                        continue;
                    }
                }
            }

            // equality on small parts
            if(ns==ng)
            {
                // the same
                return 0;
            }
            else
            {
                // small<great
                return exch ? 1:-1;
            }
        }

        template <> int  string<CH>:: compare(const string &lhs, const string &rhs) noexcept
        {
            return cmp(static_cast<const CH*>(lhs.block), lhs.chars,
                       static_cast<const CH*>(rhs.block), rhs.chars);
        }

        template <> int  string<CH>:: callcmp(const string &lhs, const string &rhs) noexcept
        {
            return cmp(static_cast<const CH*>(lhs.block), lhs.chars,
                       static_cast<const CH*>(rhs.block), rhs.chars);
        }

        template <> int  string<CH>:: callcmp(const string &lhs, const CH *rhs) noexcept
        {
            return cmp(static_cast<const CH*>(lhs.block), lhs.chars,
                       rhs, length_of(rhs) );
        }

        template <> int  string<CH>:: callcmp(const CH *lhs, const string &rhs) noexcept
        {
            return cmp(lhs, length_of(lhs),
                       static_cast<const CH*>(rhs.block), rhs.chars);
        }



        template <>
        CH  &string<CH>:: front() noexcept
        {
            assert(size()>0);
            return item[1];
        }

        template <>
        const CH  &string<CH>:: front() const noexcept
        {
            assert(size()>0);
            return item[1];
        }

        template <>
        CH  &string<CH>:: back() noexcept
        {
            assert(size()>0);
            return item[chars];
        }

        template <>
        const CH  &string<CH>:: back() const noexcept
        {
            assert(size()>0);
            return item[chars];
        }

    }
}
