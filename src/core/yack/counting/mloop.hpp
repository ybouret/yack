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

        protected:
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
    class mloop :  public counting, public kernel::mloop,public readable<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                           //!< aliases

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
        counting(1),
        kernel::mloop(dim),
        values(0),
        origin(0),
        target(0),
        moving(0),
        stride(0)
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
        // other methods
        //______________________________________________________________________

        //! reset value with SAME dims
        void reset(const T     *ini,
                   const T     *end)
        {
            assert(ini);
            assert(end);
            setup_status(ini,end);
            coerce(index)=1;
        }

        //! helper
        void display() const
        {
            std::cerr << "<mloop #levels=" << levels << ", #total=" << total << ", granted=" << granted() << ">" << std::endl;
            for(size_t i=1;i<=levels;++i)
            {
                std::cerr << "#" << i << " : " << origin[i] << " -> " << target[i] << std::endl;
            }
            std::cerr << "<mloop/>" << std::endl;
        }


    protected:
        mutable_type   *values; //!< current [1..size()] values
        const_type     *origin; //!< origin  [1..size()] values
        const_type     *target; //!< target  [1..size()] values

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mloop);
        typedef void  (*change)(mutable_type&);
        const bool     *moving;
        change         *stride;


        inline void setup_memory()
        {
            memory::embed emb[] =
            {
                memory::embed(values,levels),
                memory::embed(moving,levels),
                memory::embed(stride,levels),
                memory::embed(origin,levels),
                memory::embed(target,levels)
            };
            allocate_cxx(emb,sizeof(emb)/sizeof(emb[0]));
        }

        inline void setup_status(const T     *ini,
                                 const T     *end)
        {
            cardinality_t &prod = coerce(total);
            prod=1;
            for(size_t i=0,j=1;i<levels;++i,++j)
            {
                const T lo = coerce(origin[j]) = ini[i];
                const T hi = coerce(target[j]) = end[i];
                switch( __sign::of(lo,hi) )
                {
                    case negative: assert(lo<hi);
                        prod *= (hi-lo+1);
                        coerce(moving[j]) = true;
                        coerce(stride[j]) = incr;
                        break;

                    case __zero__: assert(lo==hi);
                        coerce(moving[j]) = false;
                        coerce(stride[j]) = NULL;
                        break;

                    case positive: assert(lo>hi);
                        prod *= (lo-hi+1);
                        coerce(moving[j]) = true;
                        coerce(stride[j]) = decr;
                        break;
                }
                values[j] = lo;
            }
        }

        static inline void incr(mutable_type &v) throw() { ++v; }
        static inline void decr(mutable_type &v) throw() { --v; }
        inline size_t      scan(size_t dim) const throw()
        {
            return ++dim > levels ? 1 : dim;
        }

        void update(const size_t odim) throw()
        {
            assert(odim>=1);
            assert(odim<=levels);

            size_t idim = odim;
        TRY_MOVE:
            if(moving[idim])
            {
                mutable_type &curr = values[idim];
                if(target[idim]==curr)
                {
                    curr=origin[idim];
                    goto NEXT_DOF;
                }

                stride[idim](curr);
                return;
            }

        NEXT_DOF:
            idim = scan(idim);
            if(odim==idim) return;
            goto TRY_MOVE;

        }

        //______________________________________________________________________
        //
        // counting interface
        //______________________________________________________________________

        //! reset initial values
        virtual void on_boot() throw()
        {
            for(size_t i=levels;i>0;--i) values[i] = origin[i];
        }

        //! find next frame
        virtual bool on_next() throw()
        {
            if(index<=total)
            {
                update(1);
                return true;
            }
            else
            {
                return false;
            }
        }

    };

}

#endif

