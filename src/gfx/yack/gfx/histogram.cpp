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

        size_t histogram:: cardinality() const throw()
        {
            size_t res = bin[0];
            for(size_t i=1;i<bins;++i)
            {
                res += bin[i];
            }
            return res;
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

        uint8_t histogram:: Otsu() const throw()
        {
            // Total number of pixels
            int total = cardinality();

            double sum = 0;
            for (int t=0 ; t<256 ; t++) sum += t * bin[t];

            double sumB = 0;
            int    wB = 0;
            int    wF = 0;

            double varMax = 0;
            int threshold = 0;

            for (int t=0 ; t<256 ; t++) {
                wB += bin[t];               // Weight Background
                if (wB == 0) continue;

                wF = total - wB;                 // Weight Foreground
                if (wF == 0) break;

                sumB += (double) (t * bin[t]);

                const double mB = sumB / wB;            // Mean Background
                const double mF = (sum - sumB) / wF;    // Mean Foreground

                // Calculate Between Class Variance
                double varBetween = (double)wB * (double)wF * (mB - mF) * (mB - mF);

                // Check if new maximum found
                if (varBetween > varMax) {
                    varMax = varBetween;
                    threshold = t;
                }
            }
            return threshold;
        }


    }

}
