#include "yack/utest/run.hpp"
#include <stdio.h>
#include <string.h>

#include "yack/color/unit-real.hpp"
#include "yack/color/rgb.hpp"

using namespace yack;


YACK_UTEST(bmp_io)
{
    randomized::rand_ ran;

    int w = 101;
    int h = 50;
    FILE *f;
    unsigned char *img = NULL;
    const int      npad = (4-(w*3)%4)%4;
    int            filesize = 54 + h*(3*w+npad);  //w is your image width, h is image height, both int

    img = (unsigned char *)malloc(3*w*h);
    memset(img,0,3*w*h);

    const color::rgb  c00(255,0,0);
    const color::rgb  c11(255,255,255);
    const color::rgb  c01(0,255,0);
    const color::rgb  c10(0,0,255);


    for(int j=0; j<h; j++)
    {
        const float y0 = float(j)/h;
        const float y1 = 1.0f-y0;

        for(int i=0; i<w; i++)
        {
            const float x0 = float(i)/w;
            const float x1 = 1.0f-x0;
            if(false)
            {
                img[ (i+j*w)*3 + 2 ] = 0xff; // r
                img[ (i+j*w)*3 + 1 ] = 0xff; // g
                img[ (i+j*w)*3 + 0 ] = 0xff; // b
            }
            else
            {
                const float w00 = x0*y0;
                const float w11 = x1*y1;
                const float w01 = x0*y1;
                const float w10 = x1*y0;

                for(int k=1;k<=3;++k)
                {
                    const float   cf = w00 * c00[k] + w01 * c01[k] + w11 * c11[k] + w10 * c10[k];
                    const uint8_t c8 = floor(cf+0.5f);
                    img[ (i+j*w)*3 + (3-k) ] = c8;
                }

            }

#if 0
            const int x=i;
            const int y=(h-1)-j;
            r = red[i][j]*255;
            g = green[i][j]*255;
            b = blue[i][j]*255;
            if (r > 255) r=255;
            if (g > 255) g=255;
            if (b > 255) b=255;
            img[(x+y*w)*3+2] = (unsigned char)(r);
            img[(x+y*w)*3+1] = (unsigned char)(g);
            img[(x+y*w)*3+0] = (unsigned char)(b);
#endif
        }
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[4] = {0,0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);

    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);

    f = fopen("img.bmp","wb");
    size_t nw = 0;
    nw += fwrite(bmpfileheader,1,14,f);
    nw += fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<h; i++)
    {
        nw += fwrite(img+(w*(h-i-1)*3),3,w,f) * 3;
        nw += fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }

    free(img);
    fclose(f);
    std::cerr << "#written=" << nw << " / " << filesize << std::endl;
}
YACK_UDONE()

