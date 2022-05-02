
//! \file

#ifndef YACK_SYNC_TESS2D_INCLUDED
#define YACK_SYNC_TESS2D_INCLUDED 1

#include "yack/concurrent/split2d.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace concurrent
    {
        
        //______________________________________________________________________
        //
        //
        //! 2D tesselation
        //
        //______________________________________________________________________
        template <typename T>
        class tess2D : public cxx_array< tiles2D<T> >
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef tiles2D<T>            tiles_type; //!< alias
            typedef cxx_array<tiles_type> array_type; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! setup
            inline explicit tess2D(const v2d<T>  &lower,
                                   const v2d<T>  &upper,
                                   const size_t   size) :
            array_type(size)
            {
                tiles_type *t = **this;
                for(size_t rank=0;rank<size;++rank)
                {
                    split2D::build(*(++t),size,rank,lower,upper);
                }
            }

            //! setup
            inline explicit tess2D(const v2d<T>   info,
                                   const size_t   size) :
            array_type(size)
            {
                const v2d<T> lower(0,0);
                const v2d<T> upper(info.x-1,info.y-1);
                tiles_type *t = **this;
                for(size_t rank=0;rank<size;++rank)
                {
                    split2D::build(*(++t),size,rank,lower,upper);
                }
            }
            
            //! cleanup
            inline virtual ~tess2D() throw() {}
                                   
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tess2D);
        };
    }
    
}

#endif
