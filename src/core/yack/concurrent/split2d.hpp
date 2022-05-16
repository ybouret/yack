//! \file

#ifndef YACK_SYNC_SPLIT2D_INCLUDED
#define YACK_SYNC_SPLIT2D_INCLUDED 1

#include "yack/concurrent/split1d.hpp"
#include "yack/type/v2d.hpp"
#include "yack/object.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/ptr/ptr.hpp"

namespace yack
{
    namespace concurrent
    {
        
        //______________________________________________________________________
        //
        //
        //! single tile: (x,y) + delta x
        //
        //______________________________________________________________________
        template <typename T>
        class tile2D  : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            inline virtual ~tile2D() throw()  {}
            
            //! setup
            inline explicit tile2D(const v2d<T> &org, const T len) throw() :
            next(0), start(org), width(len) {}
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            tile2D       *next;  //!< for tiles
            const v2d<T>  start; //!< (x,y)
            const T       width; //!< delta x
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tile2D);
        };
        
        struct split2D;

        //______________________________________________________________________
        //
        //
        //! list of tiles
        //
        //______________________________________________________________________
        template <typename T>
        class tiles2D
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef tile2D<T> tile_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline  tiles2D() throw() : items(0), tiles() {} //!< setup
            inline ~tiles2D() throw() {}                     //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! add a new tile, update items
            inline void add(const v2d<T> &org, const T len)
            {
                coerce(items) += tiles.store( new tile2D<T>(org,len) )->width;
            }
            
            inline size_t           size() const throw() { return tiles.size; } //!< number of tiles
            inline const tile_type *head() const throw() { return tiles.head; } //!< first tile
            
            //__________________________________________________________________
            //
            //! code to loop over all vertices
            //__________________________________________________________________
            
#define YACK_SYNC_TILES2D(CODE)                                   \
/**/  for(const concurrent::tile2D<T> *t=tiles.head;t;t=t->next) \
/**/  {                                                         \
/**/    unit_t len = t->width;                                 \
/**/    v2d<T> pos = t->start;                                \
/**/    while(len-- >0)                                      \
/**/    {                                                   \
/**/      CODE;                                            \
/**/      ++pos.x;                                        \
/**/    }                                                \
/**/  }
            
            //__________________________________________________________________
            //
            //! apply proc(vertex)
            //__________________________________________________________________
            template <typename PROC>
            void apply(PROC &proc) const
            {
                YACK_SYNC_TILES2D(proc(pos))
            }
            
            //__________________________________________________________________
            //
            //! apply proc(vertex,args)
            //__________________________________________________________________
            template <typename PROC, typename ARGS>
            void apply(PROC &proc, ARGS &args) const
            {
                YACK_SYNC_TILES2D(proc(pos,args))
            }

            //__________________________________________________________________
            //
            //! display
            //__________________________________________________________________
            friend inline std::ostream & operator<<(std::ostream &os, const tiles2D &self)
            {
                os << '{';
                const tile_type *t = self.head();
                if(t)
                {
                    show1(os,t);
                    for(t=t->next;t;t=t->next)
                        show1(os << '|',t);
                }
                else
                {
                    os << ptr_::nil;
                }
                os << '}';
                return os;
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t   items; //!< total items
            
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tiles2D);
            cxx_pool_of< tile_type > tiles;
            inline void   finish()     throw() { tiles.reverse();   } //!< reverse order
            friend struct split2D;

            static inline void show1(std::ostream &os, const tile_type *t)
            {
                os << t->start << "+" << t->width;
            }


        };
        
        
        
        //______________________________________________________________________
        //
        //
        //! generic splitting 2D algorithm
        //
        //______________________________________________________________________
        struct split2D
        {
            //__________________________________________________________________
            //
            //! x + y*width.x = offset
            //__________________________________________________________________
            template <typename U> static inline
            v2d<U> offset_to_vertex(const U offset, const v2d<U> &width) throw()
            {
                const U y = offset/width.x; assert(y<width.y);
                return v2d<U>(offset-y*width.x,y);
            }
            
            //__________________________________________________________________
            //
            //! build tiles for size.rank
            //__________________________________________________________________
            template <typename T, typename U> static inline
            void build(tiles2D<U>     &tiles,
                       const T         size,
                       const T         rank,
                       const v2d<U>   &lower,
                       const v2d<U>   &upper) throw()
            {
                assert(size>0);
                assert(rank<size);
                assert(lower.x<=upper.x);
                assert(lower.y<=upper.y);
                assert(0==tiles.size() );
                
                //______________________________________________________________
                //
                // split all space
                //______________________________________________________________
                const v2d<U> width(1+upper.x-lower.x,1+upper.y-lower.y);
                U            length = width.x*width.y;
                U            offset = 0;
                split1D::with(size,rank,length,offset);
                
                
                if(length>0)
                {
                    //__________________________________________________________
                    //
                    // extract tiles
                    //__________________________________________________________
                    --length;
                    const v2d<U> ini = lower + offset_to_vertex(offset,width);
                    const v2d<U> end = lower + offset_to_vertex(offset+length,width);
                    if(ini.y<end.y)
                    {
                        //______________________________________________________
                        //
                        // first
                        //______________________________________________________
                        tiles.add( ini, 1+upper.x-ini.x );
                        
                        //______________________________________________________
                        //
                        // middle
                        //______________________________________________________
                        v2d<U> pos(lower.x,ini.y+1);
                        while(pos.y<end.y)
                        {
                            tiles.add(pos,width.x);
                            ++pos.y;
                        }
                        
                        //______________________________________________________
                        //
                        // last
                        //______________________________________________________
                        tiles.add( v2d<U>(lower.x,end.y), 1+end.x-lower.x );
                        
                        //______________________________________________________
                        //
                        // set in increasing order
                        //______________________________________________________
                        tiles.finish();
                        assert(size_t(length+1)==tiles.items);
                    }
                    else
                    {
                        //______________________________________________________
                        //
                        // single tile
                        //______________________________________________________
                        tiles.add(ini,1+end.x-ini.x);
                        assert(tiles.head()->width==length+1);
                    }
                }
                else
                {
                    //__________________________________________________________
                    //
                    // no tiles
                    //__________________________________________________________
                }
            }
            
        };
        
    }
    
}

#endif
