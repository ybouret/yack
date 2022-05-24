
#include "yack/gfx/blob.hpp"
 
namespace yack
{
    namespace graphic
    {

        blob:: ~blob() throw()
        {
        }

        blob:: blob(const size_t i) throw() :
        indx(i),
        next(0),
        prev(0),
        cnodes()
        {
        }
        
        void blob:: insert(const coord &pos)
        {
            cnodes.append_back(pos);
        }

        blob * blobs:: acquire()
        {
            return content.push_back( new blob(content.size+1) );
        }

        void blobs:: release() throw()
        {
            content.release();
        }

    }
}

#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace graphic
    {

        blobs:: blobs() throw() : content()
        {

        }

        blobs:: ~blobs() throw()
        {
        }

        static inline
        int compare_blobs(const blob *lhs, const blob *rhs) throw()
        {
            return comparison::decreasing<size_t>( (**lhs).size, (**rhs).size );
        }

        void blobs:: relabel() throw()
        {
            size_t i = 0;
            for(blob *b=content.head;b;b=b->next)
            {
                coerce(b->indx) = ++i;
            }
        }

        void   blobs:: sort()
        {
            merge_list_of<blob>::sort(content, compare_blobs);
            relabel();
        }

        void blobs:: no_smaller_than(const size_t minsize, blob::chart &bmap)
        {
            while(content.size)
            {
                const cnode::list &V = **content.tail;
                if(V.size>minsize) return;
                for(const cnode *v=V.head;v;v=v->next)
                {
                    bmap(**v) = 0;
                }
                delete content.pop_back();
            }
        }


    }

}
