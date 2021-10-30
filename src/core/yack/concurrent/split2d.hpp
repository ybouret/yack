

//! \file

#ifndef YACK_SYNC_SPLIT2D_INCLUDED
#define YACK_SYNC_SPLIT2D_INCLUDED 1

#include "yack/concurrent/split1d.hpp"
#include "yack/type/v2d.hpp"
#include "yack/object.hpp"
#include "yack/data/cxx-pool.hpp"

namespace yack
{
    namespace concurrent
    {
        
        template <typename T>
        class tile  : public object
        {
        public:
            inline virtual ~tile() throw() {}
            inline explicit tile(const v2d<T> &org, const T len) throw() :
            next(0),
            start(org),
            width(len)
            {
            }
            
            tile         *next;
            const v2d<T>  start;
            const T       width;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tile);
        };
        
        template <typename T>
        class tiling : public cxx_pool_of< tile<T> >
        {
        public:
            explicit tiling() throw() {}
            virtual ~tiling() throw() {}
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tiling);
        };
        
        
        //______________________________________________________________________
        //
        //
        //! generic splitting 2D algorithm
        //
        //______________________________________________________________________
        struct split2D
        {
            
            // x + y*width.x = offset
            template <typename U> static inline
            v2d<U> offset_to_vertex(const U offset, const v2d<U> &width) throw()
            {
                const U y = offset/width.x; assert(y<width.y);
                return v2d<U>(offset-y*width.x,y);
            }
            
            
            //! from ...
            template <typename T, typename U> static inline
            void build(tiling<U>    &tiles,
                       const T       size,
                       const T       rank,
                       const v2d<U> &lower,
                       const v2d<U> &upper) throw()
            {
                assert(size>0);
                assert(rank<size);
                assert(lower.x<=upper.x);
                assert(lower.y<=upper.y);
                assert(0==tiles.size);
                
                const v2d<U> width(1+upper.x-lower.x,1+upper.y-lower.y);
                U            length = width.x*width.y;
                U            offset = 0;
                split1D::with(size,rank,length,offset);
             
                std::cerr << "@" << size << "." << rank << ": length=" << length << std::endl;
                if(length>0)
                {
                    --length;
                    const v2d<U> ini = lower + offset_to_vertex(offset,width);
                    const v2d<U> end = lower + offset_to_vertex(offset+length,width);
                    std::cerr << "\tini@" << ini << ", end@" << end << std::endl;
                    if(ini.y<end.y)
                    {
                        // multiple tiles
                    }
                    else
                    {
                        // single tile
                        tiles.store( new tile<U>(ini,1+end.x-ini.x) );
                        assert(tiles.head->width==length+1);
                    }
                }
                else
                {
                    // no tiles
                }
            }
                      
        };

    }

}

#endif
