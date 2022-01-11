#include "yack/memory/embed.hpp"
#include "yack/memory/allocator.hpp"
#include "yack/arith/align.hpp"

#include <iostream>
#include <iomanip>

namespace yack
{
    namespace memory
    {
        embed::~embed() throw()
        {
        }
        
        embed::embed(const embed &other) throw() :
        handle(other.handle),
        offset(other.offset),
        length(other.length),
        width_(other.width_)
        {
        }

        void embed:: cxx() throw()
        {
            assert(handle);
            const ptrdiff_t offset = -ptrdiff_t(width_);
            *handle = out_of_reach::shift(*handle,offset);
        }

        void  embed:: cxx(embed emb[],const size_t num) throw()
        {
            assert(!(0==emb&&num>0));
            for(size_t i=0;i<num;++i) emb[i].cxx();
        }

        std::ostream & operator<<(std::ostream &os, const embed &emb)
        {
            assert(emb.handle);
            os << (*emb.handle) << " @" << std::setw(6) << emb.offset;
            os << "+" << std::setw(6) << emb.length;
            os << "#" << std::setw(6) << emb.length/emb.width_;
            return os;
        }
        
        void * embed:: zalloc(embed emb[], const size_t num, allocator &mem, size_t &bs)
        {
            assert(!(0==emb&&num>0));
            bs = 0;
            if(num>0)
            {
                //______________________________________________________________
                //
                //
                // PASS 1/2: compute offset+length
                //
                //______________________________________________________________
                {
                    embed *prev  = emb;
                    prev->offset = 0;
                    prev->length = YACK_MEMALIGN(emb[0].length);
                    for(size_t i=1;i<num;++i)
                    {
                        embed  *curr = &emb[i];
                        curr->offset = prev->offset + prev->length;
                        curr->length = YACK_MEMALIGN(curr->length);
                        prev = curr;
                    }
                    bs = prev->offset+prev->length;
                }
                uint8_t *wksp = static_cast<uint8_t*>( mem.acquire(bs,1) );

                //______________________________________________________________
                //
                //
                // PASS 2/2: link
                //
                //______________________________________________________________
                for(size_t i=0;i<num;++i)
                {
                    embed &e = emb[i]; assert(e.handle);
                    *(e.handle) = &wksp[e.offset];
                }
                
                return wksp;
            }
            else
            {
                return NULL;
            }
        }
        
    }
    
}
