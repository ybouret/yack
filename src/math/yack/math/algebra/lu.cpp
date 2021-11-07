
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
        
        void lu_:: clear(size_t n) throw()
        {
            assert(kill);
            while(n-- > 0)
            {
                kill(data+step*n);
            }
        }
        
        lu_:: ~lu_() throw()
        {
            static memory::allocator &mem = memory::dyadic::location();
            clear(dims);
            mem.release(wksp,wlen);
        }
        
        lu_::lu_(const size_t nmax,
                 const size_t itsz,
                 proc         make,
                 proc         done) :
        dims(nmax),
        dneg(false),
        upos(NULL),
        data(NULL),
        step(itsz),
        kill(done),
        wksp(NULL),
        wlen(0)
        {
            static memory::allocator &mem = memory::dyadic::instance();
            assert(nmax>0);
            assert(itsz>0);
            assert(make);
            assert(done);
            
            //------------------------------------------------------------------
            //
            // linear memory
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
            
            //------------------------------------------------------------------
            //
            // create objects
            //
            //------------------------------------------------------------------
            size_t built = 0;
            try {
                while(built<dims)
                {
                    make(data+step*built);
                    ++built;
                }
            }
            catch(...)
            {
                clear(built);
                mem.release(wksp,wlen);
                throw;
            }
            --upos;
        }
        
        size_t lu_:: size() const throw()
        {
            return dims;
        }
        
        void *lu_:: get_scal() throw()
        {
            return data;
        }

        
    }
}
