
#include "yack/string.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/buffer/fixed.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include "yack/system/endian.hpp"
#include "yack/memory/buffer/io.hpp"

using namespace yack;


YACK_UTEST(memory_buff_io)
{
    if(false)
    {
        string                      src = "Hello World!\n";
        const memory::ro_buffer    &bf = src;
        const memory::fixed_buffer  fb( src(), src.size() );
        bf.display_char(std::cerr) << std::endl;
        fb.display_char(std::cerr) << std::endl;
    }
    
    {
        const uint32_t source = 0xF1F2F2F4;
        const memory::fixed_buffer  fb(&source,sizeof(source));
        std::cerr << ios::hexa(source) << std::endl;
        fb.display_char(std::cerr) << std::endl;
        const void *addr = fb.ro_addr();
        size_t      size = fb.measure();
        uint32_t    dw   = memory::buffer_io::get<uint32_t>(addr,size);
        std::cerr << ios::hexa(dw) << std::endl;
    }
    
    {
        const uint32_t source[2] =  { 0xF1F2F2F4, 0xEAEBECED };
        YACK_CHECK(sizeof(source)==8);
        const memory::fixed_buffer  fb( &source[0],sizeof(source));
        std::cerr << ios::hexa(source[0]) << ios::hexa(source[1]) << std::endl;
        fb.display_char(std::cerr) << std::endl;
        const void *addr = fb.ro_addr();
        size_t      size = fb.measure();
        uint32_t    dw   = memory::buffer_io::get<uint32_t>(addr,size);
        std::cerr << ios::hexa(dw) << std::endl;
        dw = memory::buffer_io::get<uint32_t>(addr,size);
        std::cerr << ios::hexa(dw) << std::endl;
    }
    
    
}
YACK_UDONE()

