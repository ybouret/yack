
#include "yack/ipso/api.hpp"
#include "yack/ios/fmt/align.hpp"

namespace yack
{
    namespace ipso
    {



        api_::  api_(container &_) noexcept :  super(_) {}
        api_:: ~api_() noexcept {}

        void api_:: disp(const rtti &outer,
                         const rtti &inner)  
        {
            std::cerr << "ipso[" << ios::align(outer.name(),width,ios::align::center)
            << "] => [" <<  ios::align(inner.name(),width,ios::align::center) << "]" << std::endl;
        }

        void api_:: ldz() noexcept
        {
            super.free();
        }


        void api_:: ldz(const size_t n)
        {
            super.resume(n);
        }

    }

}

