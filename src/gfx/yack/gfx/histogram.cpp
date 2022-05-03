#include "yack/gfx/histogram.hpp"
#include <cstring>

namespace yack
{
    namespace graphic
    {

        histogram:: ~histogram() throw()
        {
            reset();
        }

        histogram:: histogram() throw() :
        bin()
        {
            reset();
        }

        void histogram:: reset() throw()
        {
            memset(bin,0,sizeof(bin));
        }

        size_t & histogram:: operator[](const uint8_t i) throw()
        {
            return bin[i];
        }

        const size_t & histogram:: operator[](const uint8_t i) const throw()
        {
            return bin[i];
        }

        void  histogram:: merge(const histogram &H) throw()
        {
            for(size_t i=0;i<bins;++i)
            {
                bin[i] += H.bin[i];
            }
        }

        histogram:: histogram(const histogram &other) throw() :
        bin()
        {
            memcpy(bin,other.bin,sizeof(bin));
        }


        histogram & histogram:: operator=(const histogram &other) throw()
        {
            memmove(bin,other.bin,sizeof(bin));
            return *this;
        }


    }

}
