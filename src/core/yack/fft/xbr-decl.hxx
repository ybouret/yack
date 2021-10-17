template <typename T> static inline
void yack_xbitrev(T data[], const size_t size) throw()
{
  assert(NULL!=data); assert(size>0);
  switch(size)
  {
    case 0:
    case 2: return;

    case 4:
      extern uint8_t yack_xbitrev_I4[1];
      extern uint8_t yack_xbitrev_J4[1];
      {
         const uint8_t *I=yack_xbitrev_I4;
         const uint8_t *J=yack_xbitrev_J4;
         for(size_t k=1;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 8:
      extern uint8_t yack_xbitrev_I8[2];
      extern uint8_t yack_xbitrev_J8[2];
      {
         const uint8_t *I=yack_xbitrev_I8;
         const uint8_t *J=yack_xbitrev_J8;
         for(size_t k=2;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 16:
      extern uint8_t yack_xbitrev_I16[6];
      extern uint8_t yack_xbitrev_J16[6];
      {
         const uint8_t *I=yack_xbitrev_I16;
         const uint8_t *J=yack_xbitrev_J16;
         for(size_t k=6;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 32:
      extern uint8_t yack_xbitrev_I32[12];
      extern uint8_t yack_xbitrev_J32[12];
      {
         const uint8_t *I=yack_xbitrev_I32;
         const uint8_t *J=yack_xbitrev_J32;
         for(size_t k=12;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 64:
      extern uint8_t yack_xbitrev_I64[28];
      extern uint8_t yack_xbitrev_J64[28];
      {
         const uint8_t *I=yack_xbitrev_I64;
         const uint8_t *J=yack_xbitrev_J64;
         for(size_t k=28;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 128:
      extern uint8_t yack_xbitrev_I128[56];
      extern uint8_t yack_xbitrev_J128[56];
      {
         const uint8_t *I=yack_xbitrev_I128;
         const uint8_t *J=yack_xbitrev_J128;
         for(size_t k=56;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 256:
      extern uint16_t yack_xbitrev_I256[120];
      extern uint16_t yack_xbitrev_J256[120];
      {
         const uint16_t *I=yack_xbitrev_I256;
         const uint16_t *J=yack_xbitrev_J256;
         for(size_t k=120;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 512:
      extern uint16_t yack_xbitrev_I512[240];
      extern uint16_t yack_xbitrev_J512[240];
      {
         const uint16_t *I=yack_xbitrev_I512;
         const uint16_t *J=yack_xbitrev_J512;
         for(size_t k=240;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 1024:
      extern uint16_t yack_xbitrev_I1024[496];
      extern uint16_t yack_xbitrev_J1024[496];
      {
         const uint16_t *I=yack_xbitrev_I1024;
         const uint16_t *J=yack_xbitrev_J1024;
         for(size_t k=496;k>0;--k)
         {
            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];
            yack::cswap(lhs[0],rhs[0]);
            yack::cswap(lhs[1],rhs[1]);
         }
      }
      return;

    case 2048:
      extern uint16_t yack_xbitrev_I2048[992];
      extern uint16_t yack_xbitrev_J2048[992];
      {
         const uint16_t *I=yack_xbitrev_I2048;
         const uint16_t *J=yack_xbitrev_J2048;
         for(size_t k=992;k>0;--k)
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
#if defined(YACK_XBITREV_TEST)
#include <iostream>
int main()
{
  for(size_t size=1;size<=8192;size<<=1) {
    std::cerr << "size=" << size << std::endl;
    float *f = new float[2*size];
    xbitrev(f-1,size);
    delete []f;
  }
}
#endif
