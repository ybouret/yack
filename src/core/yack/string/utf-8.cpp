

#include "yack/string/utf-8.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

#include <iostream>

namespace yack
{
    const char UTF8:: clid[] = "UTF-8";
    
    
    
    const UTF8::range<UTF8::code_t> UTF8::code_bank[num_banks] =
    {
        { 0x00000000, 0x0000007f },
        { 0x00000080, 0x000007ff },
        { 0x00000800, 0x0000FFFF },
        { 0x00010000, 0x0010FFFF }
    };
    
    const UTF8::range<UTF8::byte_t> UTF8::byte_bank[num_banks] =
    {
        { 0x00, 0x7f },
        { 0xc2, 0xdf },
        { 0xe0, 0xef },
        { 0xf0, 0xf4 }
    };
    
    const UTF8:: range<UTF8::byte_t> UTF8::continuation = { 0x80, 0xbf };
    
    size_t UTF8:: validate(code_t &codepoint)
    {
        for(size_t i=0,n=1;i<num_banks;++i,++n)
        {
            if(code_bank[i].owns(codepoint))
            {
                codepoint |= (n<<info_move);
                return n;
            }
        }
        const uint32_t temp = codepoint;
        codepoint = 0;
        throw libc::exception(EINVAL,"%s invalid codepoint U+%x",clid,temp);
    }
    
    UTF8:: UTF8(const code_t codepoint) :
    code(codepoint)
    {
        (void) validate(code);
    }
    
    UTF8:: ~UTF8() throw()
    {
        code=0;
    }
    
    UTF8:: UTF8(const UTF8 &U) throw() : code(U.code)
    {
    }
    
    UTF8 & UTF8::operator=(const UTF8 &U) throw()
    {
        code=U.code;
        return *this;
    }
    
    UTF8 & UTF8::operator=( code_t codepoint)
    {
        (void)validate(codepoint);
        code=codepoint;
        return *this;
    }
  
    size_t UTF8:: bytes() const throw()
    {
        const size_t res = (code>>info_move);
        assert(res>=1); assert(res<=4);
        return res;
    }

    UTF8::code_t UTF8:: operator*() const throw()
    {
        return (code&code_mask);
    }
}


