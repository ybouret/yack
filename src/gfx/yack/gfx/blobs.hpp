
//! \file

#ifndef YACK_GFX_BLOBS_INCLUDED
#define YACK_GFX_BLOBS_INCLUDED 1

#include "yack/gfx/blob.hpp"
#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/pixel.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! pixmap of tags
        //
        //______________________________________________________________________
        typedef pixmap<size_t> tagmap;


        typedef cxx_list_of<blob> blobs_; //!< alias

        //______________________________________________________________________
        //
        //
        //! list of blobs
        //
        //______________________________________________________________________
        class blobs : public blobs_
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //__________________________________________________________________
            virtual ~blobs() noexcept; //!< setup empty
            explicit blobs() noexcept; //!< cleanup

            //------------------------------------------------------------------
            //
            // methods
            //__________________________________________________________________
            
            //! build blobs and matching tag map for cnx=4 or cnx=8
            template <typename T> inline
            void build(tagmap &tmap, broker &dev, const pixmap<T> &source, const unsigned cnx)
            {
                static const size_t z = 0;

                assert( tmap.matches_dimensions(source) );
                assert( 4==cnx || 8==cnx );

                //--------------------------------------------------------------
                //
                // cleanup
                //
                //--------------------------------------------------------------
                release();
                tmap.ld(dev,z);

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                blob        *b = NULL;     // current blob
                const unit_t h = tmap.h;   // height
                const unit_t w = tmap.w;   // width
                coord        r(0,0);       // running pos
                cnodes       vstk;         // local stack

                //--------------------------------------------------------------
                //
                // loop over source
                //
                //--------------------------------------------------------------
                for(r.y=0;r.y<h;++r.y)
                {
                    pixrow<size_t>  &row = tmap(r.y);
                    const pixrow<T> &src = source(r.y);
                    for(r.x=0;r.x<w;++r.x)
                    {
                        size_t &B = row(r.x);
                        if( pixel<T>::is_zero(src(r.x)) )
                        {
                            //--------------------------------------------------
                            // skip, frontier
                            //--------------------------------------------------
                            assert(0==B);
                            continue;
                        }

                        if(B>0)
                        {
                            //--------------------------------------------------
                            // skip, already in another blob
                            //--------------------------------------------------
                            continue;
                        }

                        //------------------------------------------------------
                        //
                        // create am initialize a new blob
                        //
                        //------------------------------------------------------
                        const size_t tag = ( b=acquire() )->tag;
                        assert(0==vstk.size);
                        vstk.append_back(r);
                        b->add(r);
                        tmap(r) = tag;

                        //------------------------------------------------------
                        //
                        // propagate
                        //
                        //------------------------------------------------------
                        while(vstk.size)
                        {
                            const coord p = vstk.pull_front(); assert( !pixel<T>::is_zero(source(p)) );
                            for(unsigned k=0;k<cnx;++k)
                            {
                                // get coordinate
                                const coord q = p+coords::delta[k]; assert(q!=p);

                                // check usage
                                if(!source.contains(q))             continue;     // outer point
                                if(pixel<T>::is_zero(source(q)))    continue;     // frontier
                                if(tmap(q)>0) { assert(tag==tmap(q)); continue; } // already in blob

                                // keep looking for
                                vstk.append_back(q);
                                tmap(q) = tag;
                                b->add(q);
                            }
                        }
                    }
                }
                
                sort();
                relabel(tmap);
            }

            //! assume blobs are in decreasing size order
            void no_smaller_than(const size_t minsize, tagmap &tmap) noexcept;

            //! re-label blobs index and tmap
            void   relabel(tagmap &tmap) noexcept;


        private:
            blob * acquire();
            void   sort();                        //!< sort in decreasing size ordere, relabel
        };


    }

}

#endif

