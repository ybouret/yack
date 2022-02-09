
#include "yack/math/algebra/lu.hpp"
#include "yack/memory/embed.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace math
    {
        
        size_t lu_:: granted() const throw()
        {
            return wlen;
        }

        
        size_t * lu_::indx_() throw()
        {
            return upos;
        }
        
        lu_:: ~lu_() throw()
        {
            static memory::allocator &mem = memory::dyadic::location();
            mem.release(wksp,wlen);
        }
        
        lu_::lu_(const size_t nmax,
                 const size_t s_sz,
                 const size_t t_sz) :
        dims(max_of<size_t>(nmax,1)),
        dneg(false),
        upos(NULL),
        sdat(NULL),
        tdat(NULL),
        wksp(NULL),
        wlen(0)
        {
            static memory::allocator &mem = memory::dyadic::instance();
            assert(dims>0);
            assert(s_sz>0);
            assert(t_sz>0);


            //------------------------------------------------------------------
            //
            // create linear memory
            //
            //------------------------------------------------------------------
            {
                memory::embed emb[] =
                {
                    memory::embed(upos,dims),
                    memory::embed(sdat,s_sz*dims),
                    memory::embed(tdat,t_sz*dims),
                };
                wksp = YACK_MEMORY_EMBED(emb,mem,wlen);
            }
            

        }
        

        
        
        
    }
}
