namespace yack {
  struct xbr {
    static const uint8_t I4[1];
    static const uint8_t J4[1];
    static const uint8_t I8[2];
    static const uint8_t J8[2];
    static const uint8_t I16[6];
    static const uint8_t J16[6];
    static const uint8_t I32[12];
    static const uint8_t J32[12];
    static const uint8_t I64[28];
    static const uint8_t J64[28];
    static const uint8_t I128[56];
    static const uint8_t J128[56];
    static const uint16_t I256[120];
    static const uint16_t J256[120];
    static const uint16_t I512[240];
    static const uint16_t J512[240];
    static const uint16_t I1024[496];
    static const uint16_t J1024[496];
  };
  template <typename T> static inline
  void xbitrev(T data[], const size_t size) throw()
  {
    assert(NULL!=data); assert(size>0);
  switch(size)
  {
    case 0:
    case 2: return;

    case 4:
      {
         const uint8_t *I=xbr::I4;
         const uint8_t *J=xbr::J4;
         for(size_t k=1;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 8:
      {
         const uint8_t *I=xbr::I8;
         const uint8_t *J=xbr::J8;
         for(size_t k=2;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 16:
      {
         const uint8_t *I=xbr::I16;
         const uint8_t *J=xbr::J16;
         for(size_t k=6;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 32:
      {
         const uint8_t *I=xbr::I32;
         const uint8_t *J=xbr::J32;
         for(size_t k=12;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 64:
      {
         const uint8_t *I=xbr::I64;
         const uint8_t *J=xbr::J64;
         for(size_t k=28;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 128:
      {
         const uint8_t *I=xbr::I128;
         const uint8_t *J=xbr::J128;
         for(size_t k=56;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 256:
      {
         const uint16_t *I=xbr::I256;
         const uint16_t *J=xbr::J256;
         for(size_t k=120;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 512:
      {
         const uint16_t *I=xbr::I512;
         const uint16_t *J=xbr::J512;
         for(size_t k=240;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 1024:
      {
         const uint16_t *I=xbr::I1024;
         const uint16_t *J=xbr::J1024;
         for(size_t k=496;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    default: // generic code
    const size_t n = (size << 1);
    for(size_t i=1,j=1;i<n;i+=2)
    {
        if(j>i)
        {
            T *lhs = &data[j];
            T *rhs = &data[i];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
        }
        size_t m=size;
        while( (m>=2) && (j>m) )
        {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
  }
  }
}
#if defined(YACK_XBITREV_TEST)
#include <iostream>
int main()
{
  for(size_t size=1;size<=4096;size<<=1) {
    std::cerr << "size=" << size << std::endl;
    float *f = new float[2*size];
    xbitrev(f-1,size);
    delete []f;
  }
}
#endif
