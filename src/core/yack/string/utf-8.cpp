

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

}


