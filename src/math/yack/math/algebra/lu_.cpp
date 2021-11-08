
#include "yack/math/algebra/lu.hpp"
#include "yack/memory/embed.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace math
    {
        
        size_t lu_:: granted() const throw()
        {
            return wlen;
        }

        
        size_t & lu_:: operator[](const size_t i) throw()
        {
            assert(i>=1);
            assert(i<=dims);
            return upos[i];
        }
        
        const size_t & lu_:: operator[](const size_t i) const throw()
        {
            assert(i>=1);
            assert(i<=dims);
            return upos[i];
        }
        

        
        lu_:: ~lu_() throw()
        {
            static memory::allocator &mem = memory::dyadic::location();
            mem.release(wksp,wlen);
        }
        
        lu_::lu_(const size_t nmax,
                 const size_t itsz) :
        dims(nmax),
        dneg(false),
        upos(NULL),
        data(NULL),
        wksp(NULL),
        wlen(0)
        {
            static memory::allocator &mem = memory::dyadic::instance();
            assert(nmax>0);
            assert(itsz>0);

            //------------------------------------------------------------------
            //
            // create linear memory
            //
            //------------------------------------------------------------------
            {
                memory::embed emb[] =
                {
                    memory::embed(upos,dims),
                    memory::embed(data,itsz*dims)
                };
                wksp = YACK_MEMORY_EMBED(emb,mem,wlen);
            }
            

            --upos;
        }
        
        size_t lu_:: size() const throw()
        {
            return dims;
        }
        
        
        
    }
}
