#define CH char
#include "string.hxx"
#include "yack/type/cstring.h"

namespace yack
{
    namespace kernel
    {

        template <> const char string<char>::clid[] = "string";
       
        template <>
        const char * string<CH>:: class_uid() const noexcept
        {
            return clid;
        }
        
       
        
        static inline
        void copy_msg(char *item, const char *msg, size_t &chars) noexcept
        {
            assert(msg);
        CYCLE:
            const char c = *(msg++);
            if(c)
            {
                item[++chars] = c;
                goto CYCLE;
            }
        }

        template <>
        string<char>:: string(const char *msg) :
        collection(),
        string_(yack_cstring_size(msg),1),
        writable<CH>(),
        item( static_cast<char*>(block)-1 )
        {
            if(msg) copy_msg(item,msg,chars);

        }

        template <>
        string<char> & string<char>:: operator=(const char *msg)
        {
            if(msg)
            {
                const size_t len = strlen(msg);
                if(len<=width)
                {
                    memcpy(block,msg,chars=len);
                    memset(out_of_reach::shift(block,ptrdiff_t(len)),0,width-chars);
                }
                else
                {
                    string<char> tmp(msg);
                    xch(tmp);
                }
            }
            else
            {
                clear();
            }
            return *this;
        }

        template <>
        string<char>  string<char>::cat(const string<char> &lhs, const char *rhs)
        {
            if(rhs)
            {
                const size_t len = strlen(rhs);
                const size_t L   = lhs.chars;
                string<char> ans(L+len,as_capacity);
                size_t      &i=ans.chars;
                for(size_t j=1;j<=L;++j)
                {
                    ans.item[++i] = lhs[j];
                }
                for(size_t j=0;j<len;++j)
                {
                    ans.item[++i] = rhs[j];
                }
                return ans;
            }
            else
            {
                return string<char>(lhs);
            }
        }

        template <>
        string<char> string<char>::cat(const char   *lhs, const string<char> &rhs)
        {
            if(lhs)
            {
                const size_t len = strlen(lhs);
                const size_t R   = rhs.chars;
                string<char> ans(len+R,as_capacity);
                size_t      &i=ans.chars;
                for(size_t j=0;j<len;++j)
                {
                    ans.item[++i] = lhs[j];
                }
                for(size_t j=1;j<=R;++j)
                {
                    ans.item[++i] = rhs[j];
                }
                return ans;
            }
            else
            {
                return string<char>(rhs);
            }

        }

        template <>
        string<char> & string<char>:: operator+=(const char *msg)
        {
            if(msg)
            {
                const size_t len = strlen(msg);
                if(chars+len<=width)
                {
                    size_t    &i=chars;
                    for(size_t j=0;j<len;++j)
                    {
                        item[++i] = msg[j];
                    }
                }
                else
                {
                    string<char> tmp = *this + msg;
                    xch(tmp);
                }
            }
            return *this;
        }

    }
}

#include "yack/ios/encoder.hpp"
#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace kernel
    {
        template <>
        size_t string<char>::serialize(ios::ostream &os) const
        {
            const size_t nw = ios::encoder::serialize<size_t>(os,chars);
            os.frame(block,chars);
            return nw + chars;
        }
    }
    
}

