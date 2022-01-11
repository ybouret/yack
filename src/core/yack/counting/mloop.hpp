//! \file

#ifndef YACK_COUNTING_MLOOP_INCLUDED
#define YACK_COUNTING_MLOOP_INCLUDED 1

#include "yack/counting/counting.hpp"
#include "yack/type/args.hpp"
#include "yack/memory/embed.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/container/readable.hpp"
#include "yack/signs.hpp"

namespace yack
{

    namespace kernel
    {
        //______________________________________________________________________
        //
        //
        //! multiple loop base class
        //
        //______________________________________________________________________
        class mloop : public dynamic
        {
        public:
            virtual       ~mloop()         throw(); //!< cleanup
            virtual size_t granted() const throw(); //!< allocated

            const size_t levels; //!< number of levels
            const size_t frames; //!< product of dimension per level
            
        protected:
            size_t        active; //!< current level

            explicit mloop(const size_t dim)             throw(); //!< setup
            void     allocate_cxx(memory::embed[], const size_t); //!< allocate

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mloop);
            void  *wksp;
            size_t wlen;
        };
    }

    //__________________________________________________________________________
    //
    //
    //! multiple loop
    //
    //__________________________________________________________________________
    template <typename T>
    class mloop : public kernel::mloop, public counting, public readable<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                           //!< aliases
        typedef bool (*shift)(mutable_type &, const_type); //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~mloop() throw() {}

        //! setup
        inline explicit mloop(const T     *ini,
                              const T     *end,
                              const size_t dim) :
        kernel::mloop(dim),
        values(0),
        moving(0),
        origin(0),
        target(0)
        {
            assert(ini);
            assert(end);
            setup_memory();
            setup_status(ini,end);
        }

        //______________________________________________________________________
        //
        // readable<T> interface
        //______________________________________________________________________

        //! size=levels
        virtual inline size_t size() const throw() { return levels; }

        //! access values
        virtual inline const_type & operator[](const size_t i) const throw()
        {
            assert(values); assert(i>=1); assert(i<=levels); return values[i];
        }

        //______________________________________________________________________
        //
        // counting interface
        //______________________________________________________________________
        virtual void boot() throw()
        {
            active = 1;
            for(size_t i=levels;i>0;--i) values[i] = origin[i];
        }

        virtual bool next() throw()
        {
            return false;
        }

        //______________________________________________________________________
        //
        // other methods
        //______________________________________________________________________

        //! helper
        void display() const
        {
            std::cerr << "<mloop #levels=" << levels << ", #frames=" << frames << ", granted=" << granted() << ">" << std::endl;
            for(size_t i=1;i<=levels;++i)
            {
                std::cerr << "#" << i << " : " << origin[i] << " -> " << target[i] << std::endl;
            }
            std::cerr << "<mloop/>" << std::endl;
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mloop);
        mutable_type *values;
        shift        *moving;
        const_type   *origin;
        const_type   *target;

        inline void setup_memory()
        {
            memory::embed emb[] =
            {
                memory::embed(values,levels),
                memory::embed(moving,levels),
                memory::embed(origin,levels),
                memory::embed(target,levels)
            };
            allocate_cxx(emb,sizeof(emb)/sizeof(emb[0]));
        }

        inline void setup_status(const T     *ini,
                          const T     *end) throw()
        {
            size_t &prod = coerce(frames);
            prod=1;
            for(size_t i=0,j=1;i<levels;++i,++j)
            {
                const T lo = coerce(origin[j]) = ini[i];
                const T hi = coerce(target[j]) = end[i];
                switch( __sign::of(lo,hi) )
                {
                    case negative: assert(lo<hi);
                        prod *= (hi-lo+1);
                        moving[j] = incr;
                        break;

                    case __zero__: assert(lo==hi);
                        moving[j] = skip;
                        break;

                    case positive: assert(lo>hi);
                        prod *= (lo-hi+1);
                        moving[j] = decr;
                        break;
                }
                values[j] = lo;
            }
        }

        static inline bool incr(mutable_type &v, const_type t) throw()
        {
           if(v<t)
           {
               ++v;
               return true;
           }
           else
           {
               return false;
           }
        }

        static inline bool decr(mutable_type &v, const type t) throw()
        {
            if(v>t)
            {
                --v;
                return true;
            }
            else
            {
                return false;
            }
        }

        static inline bool skip(mutable_type &, const type) throw()
        {
            return false;
        }

    };

}

#endif

