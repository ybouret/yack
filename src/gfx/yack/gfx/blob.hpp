//! \file

#ifndef YACK_GFX_BLOB_INCLUDED
#define YACK_GFX_BLOB_INCLUDED 1

#include "yack/gfx/pixel.hpp"
#include "yack/memory/small/exclusive.hpp"
#include "yack/data/list/concrete.hpp"
#include "yack/data/pool/concrete.hpp"
#include "yack/object.hpp"
#include "yack/gfx/pixmap.hpp"

namespace yack
{
    namespace graphic
    {
        //! vertex node
        class vnode
        {
        public:
            YACK_EXCLUSIVE_DECL(vnode,999);
            
            vnode       * next;
            vnode       * prev;
            const coord & operator*() throw();
            const coord & operator*() const throw();

            vnode(const coord &) throw();
            ~vnode() throw();
            vnode(const vnode &) throw();

        private:
            YACK_DISABLE_ASSIGN(vnode);
            const  coord  pos;
        };

        typedef klist<const coord,vnode> vlist;
        typedef kpool<const coord,vnode> vpool;


        class blob : public object
        {
        public:
            typedef cxx_list_of<blob> list_type;
            typedef pixmap<size_t>    chart;

            const vlist & operator*() const throw() { return vertices; }

            explicit blob(const size_t) throw();
            virtual ~blob() throw();
            void     push(const coord &);

            const size_t indx; //!< index>0
            blob        *next; //!< for list
            blob        *prev; //!< for list


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(blob);
            vlist vertices;
        };

        class blobs
        {
        public:
            
            virtual ~blobs() throw();
            explicit blobs() throw();

            const blob::list_type & operator*() const throw() { return content; }


            template <typename T> inline
            void build(blob::chart &bmap, broker &dev, const pixmap<T> &source, const unsigned cnx)
            {
                static const size_t z = 0;

                assert( bmap.matches_dimensions(source) );
                assert( 4==cnx || 8==cnx );
                
                //--------------------------------------------------------------
                //
                // cleanup
                //
                //--------------------------------------------------------------
                release();
                bmap.ld(dev,z);

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                blob        *b = NULL;     // current blob
                const unit_t h = bmap.h;   // height
                const unit_t w = bmap.w;   // width
                coord        r(0,0);       // running pos
                vlist        vstk;         // local stack

                //--------------------------------------------------------------
                //
                // loop over source
                //
                //--------------------------------------------------------------
                for(r.y=0;r.y<h;++r.y)
                {
                    pixrow<size_t>  &row = bmap(r.y);
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

                        //--------------------------------------------------------------
                        //
                        // create a blob
                        //
                        //--------------------------------------------------------------
                        const size_t i = ( b=acquire() )->indx;
                        assert(0==vstk.size);
                        vstk.append_back(r);
                        b->push(r);
                        bmap(r) = i;

                        while(vstk.size)
                        {
                            const coord p = vstk.pull_front(); assert( !pixel<T>::is_zero(source(p)) );
                            for(unsigned k=0;k<cnx;++k)
                            {
                                const coord q = p+coords::delta[k]; assert(q!=p);
                                if(!source.contains(q))             continue;   //! outer point
                                if(pixel<T>::is_zero(source(q)))    continue;   //! frontier
                                if(bmap(q)>0) { assert(i==bmap(q)); continue; } //! already in blob
                                vstk.append_back(q);
                                bmap(q) = i;
                                b->push(q);
                            }
                        }
                    }
                }

                sort();
            }

            void no_smaller_than(const size_t minsize, blob::chart &bmap);


        private:
            blob::list_type content;
            blob * acquire();
            void   release() throw();
            void   sort();
            void   relabel() throw();
        };



    }
}

#endif

