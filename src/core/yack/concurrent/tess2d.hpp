
//! \file

#ifndef YACK_SYNC_TESS2D_INCLUDED
#define YACK_SYNC_TESS2D_INCLUDED 1

#include "yack/concurrent/split2d.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace concurrent
    {
        
        template <typename T>
        class tess2D : public cxx_array< tiles2D<T> >
        {
        public:
            typedef tiles2D<T> tiles_type;
            
            inline explicit tess2D(const v2d<T>  &lower,
                                   const v2d<T>  &upper,
                                   const size_t   size) :
            cxx_array< tiles2D<T> >(size)
            {
                tiles_type *t = **this;
                for(size_t rank=0;rank<size;++rank)
                {
                    split2D::build(*(++t),size,rank,lower,upper);
                }
            }
            
            inline virtual ~tess2D() throw()
            {
                
            }
                                   
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tess2D);
        };
    }
    
}

#endif
