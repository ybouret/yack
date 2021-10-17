#include "yack/fft/xbr-defs.hpp"

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cassert>

//------------------------------------------------------------------------------
//
// generic code
//
//------------------------------------------------------------------------------

static const char code[] =
"    const size_t n = (size << 1);\n"
"    for(size_t i=1,j=1;i<n;i+=2)\n"
"    {\n"
"        if(j>i)\n"
"        {\n"
"            T *lhs = &data[j];\n"
"            T *rhs = &data[i];\n"
"            yack::cswap(lhs[0],rhs[0]);\n"
"            yack::cswap(lhs[1],rhs[1]);\n"
"        }\n"
"        size_t m=size;\n"
"        while( (m>=2) && (j>m) )\n"
"        {\n"
"            j -= m;\n"
"            m >>= 1;\n"
"        }\n"
"        j += m;\n"
"    }\n";


//------------------------------------------------------------------------------
//
// write table
//
//------------------------------------------------------------------------------
static inline void output(FILE *src, const unsigned *K, const unsigned n)
{
    fprintf(src,"={\n");
    unsigned k=0;
    fprintf(src,"%u",K[k]);
    for(++k;k<n;++k)
    {
        fprintf(src,", %u",K[k]);
    }
    fprintf(src,"\n");
    fprintf(src,"};\n\n");
    
}

//------------------------------------------------------------------------------
//
// build files
//
//------------------------------------------------------------------------------
static inline void build(FILE *hdr, FILE *src)
{
    assert(hdr);
    assert(src);
    
    //--------------------------------------------------------------------------
    //
    // source prolog
    //
    //--------------------------------------------------------------------------
    fprintf(src,"#include \"yack/system/setup.h\"\n");
    
    //--------------------------------------------------------------------------
    //
    // header prolog
    //
    //--------------------------------------------------------------------------
    
    fprintf(hdr,"template <typename T> static inline\n");
    fprintf(hdr,"void yack_xbitrev(T data[], const size_t size) throw()\n");
    fprintf(hdr,"{\n");
    fprintf(hdr,"  assert(NULL!=data); assert(size>0);\n");
    
    //--------------------------------------------------------------------------
    //
    // cases
    //
    //--------------------------------------------------------------------------
    fprintf(hdr,"  switch(size)\n");
    fprintf(hdr,"  {\n");
    fprintf(hdr,"    case 0:\n");
    fprintf(hdr,"    case 2: return;\n\n");
    
    //--------------------------------------------------------------------------
    //
    // loop
    //
    //--------------------------------------------------------------------------
    for(unsigned size=4;size<=YACK_FFT_XBITREV_MAX;size<<=1)
    {
        fprintf(hdr,"    case %u:\n",size);
        
        //----------------------------------------------------------------------
        // count
        //----------------------------------------------------------------------
        unsigned       count = 0;
        unsigned       jmax  = 0;
        const unsigned n = (size << 1);
        for(unsigned i=1,j=1;i<n;i+=2)
        {
            if(j>i)
            {
                ++count;
                if(j>jmax) jmax=j;
            }
            unsigned m=size;
            while( (m>=2) && (j>m) )
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        
        //----------------------------------------------------------------------
        // prepare type
        //----------------------------------------------------------------------
        const char       *type = "uint8_t";
        size_t            bpi  = 1;
        if(jmax>=256)   { type = "uint16_t"; bpi=2; }
        if(jmax>=65536) { type = "uint32_t"; bpi=4; }
        
        {
            std::cerr << "size=" << std::setw(6) << size;
            std::cerr << " | swap=" << std::setw(6) << count;
            std::cerr << " | type=" << std::setw(8) << type;
            std::cerr << " | I/J =" << std::setw(6) << count*bpi;
            std::cerr << " | I+J =" << std::setw(6) << 2*count*bpi;

           //<< ", count=" << count << ", jmax=" << jmax << " => " << type << std::endl;
            
            std::cerr << std::endl;
        }
        //----------------------------------------------------------------------
        // declare tables in header
        //----------------------------------------------------------------------
        fprintf(hdr,"      extern %s yack_xbitrev_I%u[%u];\n",type,size,count);
        fprintf(hdr,"      extern %s yack_xbitrev_J%u[%u];\n",type,size,count);

        //----------------------------------------------------------------------
        // generate tables data
        //----------------------------------------------------------------------
        unsigned *I = new unsigned[2*count];
        unsigned *J = I+count;
        size_t    k = 0;
        for(unsigned i=1,j=1;i<n;i+=2)
        {
            if(j>i)
            {
                I[k] = i;
                J[k] = j;
                ++k;
            }
            unsigned m=size;
            while( (m>=2) && (j>m) )
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        assert(count==k);
        
        //----------------------------------------------------------------------
        // write in source
        //----------------------------------------------------------------------
        fprintf(src,"const %s yack_xbitrev_I%u[%u]",type,size,count);
        output(src,I,count);
        fprintf(src,"const %s yack_xbitrev_J%u[%u]",type,size,count);
        output(src,J,count);
        delete []I;
        
        //----------------------------------------------------------------------
        // use in table
        //----------------------------------------------------------------------
        fprintf(hdr,"      {\n");
        fprintf(hdr,"         const %s *I=yack_xbitrev_I%u;\n",type,size);
        fprintf(hdr,"         const %s *J=yack_xbitrev_J%u;\n",type,size);
        fprintf(hdr,"         for(size_t k=%u;k>0;--k)\n",count);
        fprintf(hdr,"         {\n");
        fprintf(hdr,"            T *lhs=&data[*(I++)], *rhs=&data[*(J++)];\n");
        fprintf(hdr,"            yack::cswap(lhs[0],rhs[0]);\n");
        fprintf(hdr,"            yack::cswap(lhs[1],rhs[1]);\n");
        fprintf(hdr,"         }\n");
        fprintf(hdr,"      }\n");
        
        
        fprintf(hdr,"      return;\n\n");
    }
    
    //--------------------------------------------------------------------------
    //
    // default
    //
    //--------------------------------------------------------------------------
    fprintf(hdr,"    default: // generic code\n");
    fputs(code,hdr);
    
    //--------------------------------------------------------------------------
    //
    // end of cases
    //
    //--------------------------------------------------------------------------
    fprintf(hdr,"  }\n");
    
    //--------------------------------------------------------------------------
    //
    // end of routine
    //
    //--------------------------------------------------------------------------
    fprintf(hdr,"}\n");
    
    //--------------------------------------------------------------------------
    //
    // tests...
    //
    //--------------------------------------------------------------------------
    fprintf(hdr,"#if defined(YACK_XBITREV_TEST)\n");
    fprintf(hdr,"#include <iostream>\n");
    fprintf(hdr,"int main()\n");
    fprintf(hdr,"{\n");
    
    // loop
    fprintf(hdr,"  for(size_t size=1;size<=%u;size<<=1) {\n", 4*YACK_FFT_XBITREV_MAX);
    fprintf(hdr,"    std::cerr << \"size=\" << size << std::endl;\n");
    fprintf(hdr,"    float *f = new float[2*size];\n");
    fprintf(hdr,"    xbitrev(f-1,size);\n");
    fprintf(hdr,"    delete []f;\n");
    fprintf(hdr,"  }\n");
    
    fprintf(hdr,"}\n");
    
    //--------------------------------------------------------------------------
    //
    // header epilog
    //
    //--------------------------------------------------------------------------
    fprintf(hdr,"#endif\n");

}

 
#include <cerrno>

int main(int argc, const char **argv)
{
    const char *program = argv[0];
    if(argc<2)
    {
        std::cerr << "usage: " << program << " header source" << std::endl;
        return -1;
    }
    const char *header = argv[1];
    const char *source = argv[2];
    FILE *hdr = fopen(header,"wb");
    if(!hdr)
    {
        std::cerr << "couldn't open " << header << " : " << strerror(errno) << std::endl;
    }
    
    FILE *src = fopen(source,"wb");
    if(!src)
    {
        fclose(hdr);
        std::cerr << "couldn't open " << source << " : " << strerror(errno) << std::endl;
    }
    
    build(hdr,src);
    
    fclose(src);
    fclose(hdr);
    
    return 0;
}
