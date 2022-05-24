#include "yack/gfx/blobs.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace graphic
    {

        blobs:: blobs() throw() : cxx_list_of<blob>()
        {

        }

        blobs:: ~blobs() throw()
        {
        }



        blob * blobs:: acquire()
        {
            return  push_back( new blob(size+1) );
        }



        static inline
        int compare_blobs(const blob *lhs, const blob *rhs) throw()
        {
            return comparison::decreasing<size_t>(lhs->size,rhs->size);
        }

        void blobs:: relabel(tagmap &tmap) throw()
        {
            size_t i = 0;
            for(blob *b=head;b;b=b->next)
            {
                coerce(b->tag) = ++i;
                for(const cnode *node=b->head;node;node=node->next)
                {
                    tmap(**node) = i;
                }
            }
        }

        void   blobs:: sort()
        {
            merge_list_of<blob>::sort(*this, compare_blobs);
        }

        void blobs:: no_smaller_than(const size_t minsize, tagmap &bmap) throw()
        {
            while(size)
            {
                const cnodes &V = *tail;
                if(V.size>minsize) return;

                for(const cnode *v=V.head;v;v=v->next)
                {
                    bmap(**v) = 0;
                }
                delete  pop_back();
            }
        }


    }

}
