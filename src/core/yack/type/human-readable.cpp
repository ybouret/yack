#include "yack/type/human-readable.hpp"
#include <iostream>
#include <cmath>

namespace yack
{

    namespace
    {
        struct hr_info
        {
            const char      radix;
            const uint64_t  scale;
        };

        static const uint64_t u1 = 1;
        static const hr_info  hr_tab[] =
        {
            { 'b', u1<<00 },
            { 'k', u1<<10 },
            { 'M', u1<<20 },
            { 'G', u1<<30 },
            { 'T', u1<<40 },
            { 'P', u1<<50 },
            { 'E', u1<<60 }
        };

        static const unsigned hr_num = sizeof(hr_tab)/sizeof(hr_tab[0]);
        static const unsigned hr_top = hr_num-1;


    }


    human_readable:: human_readable(const uint64_t qw) noexcept :
    ip(0),
    fp(0),
    rx(0)
    {
        std::cerr << "encoding [" << qw << "]" << std::endl;
        unsigned i=0;
        for(;i<hr_top;++i)
        {
            if(qw<hr_tab[i+1].scale)
            {
                break;
            }
        }

        const uint64_t s = hr_tab[i].scale;
        const uint64_t q = qw/s;
        const uint64_t r = qw - q*s;
        std::cerr << "s=" << s << std::endl;
        std::cerr << "q=" << q << ", r=" << r << std::endl;

        const double ratio = double(r) / s;
        std::cerr << "ratio=" << ratio << std::endl;
        coerce(ip) = unsigned(q);
        coerce(fp) = floor( 10000 * (double(r) / s) );
        coerce(rx) = hr_tab[i].radix;
        std::cerr << ip << '.' << fp << rx << std::endl;
    }

    human_readable:: ~human_readable() noexcept
    {

    }

}
