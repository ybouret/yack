#include "yack/gfx/histogram.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"
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

        void histogram:: save(const string &filename) const
        {
            ios::ocstream fp(filename);
            for(unsigned i=0;i<bins;++i)
            {
                fp("%u %g\n",i,double(bin[i]));
            }
        }


        void histogram:: save(const char  *filename) const
        {
            const string _(filename);
            save(_);
        }

    }

}
