
#include "yack/ipso/api.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace ipso
    {



        api_::  api_(container &_) noexcept : large_object(), super(_) {}
        api_:: ~api_() noexcept {}

        void api_:: disp(const rtti &outer,
                         const rtti &inner)  
        {
            std::cerr << "ipos<" << outer.name() << "> => <" << inner.name() << ">" << std::endl;
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

