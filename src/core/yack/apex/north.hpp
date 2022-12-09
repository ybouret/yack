
//! \file

#ifndef YACK_APEX_NORTH_INCLUDED
#define YACK_APEX_NORTH_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/memory/embed.hpp"
#include "yack/container/dynamic.hpp"

namespace yack
{

    namespace north
    {
        //! family maturity
        enum maturity
        {
            in_progress, //! size < dims-1
            almost_done, //! size == dims-1, next vector is unique!
            fully_grown  //! size == dims
        };

        //! initialize maturity from a dimension and a size
        struct constellation
        {
            static size_t   checked_dimension(const size_t d);
            static maturity initial_situation(const size_t dims);
            static maturity updated_situation(const size_t dims, const size_t size) throw();
        };

        template <typename T> struct classify;

        template < > struct classify<apq>
        {
            typedef apq type;
            typedef apn l2_type;
        };

        template < > struct classify<int64_t>
        {
            typedef int64_t  type;
            typedef uint64_t l2_type;
        };


        template <typename T> class qvector : public readable<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef typename classify<T>::l2_type    l2_type;    //!< alias
            typedef typename readable<T>::type       type;       //!< alias
            typedef typename readable<T>::const_type const_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup concerned area
            inline explicit qvector(type *ptr, const size_t num) :
            dimension(num), coeff(ptr), norm2(0)
            {
                assert( yack_good(ptr,num) );
                --coeff;
                init();
            }

            inline virtual ~qvector() throw() { quit(dimension); }

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            inline virtual size_t size() const throw() { return dimension; } //!< size = matrix.dimension

            inline virtual const_type & operator[](size_t indx) const throw()
            {
                assert(indx>=1); assert(indx<=dimension); return coeff[indx];
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t dimension;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qvector);
            type        *coeff;

        public:
            const type   norm2;

        private:
            inline void quit(size_t done) throw()
            {
                type  *addr = coeff+1;
                while(done-- > 0) out_of_reach::naught( destructed(addr+done) );
            }

            inline void init() {
                size_t done = 0;
                type  *addr = coeff+1; assert(out_of_reach::is0(addr,dimension*sizeof(type)));
                try {
                    while(done<dimension) {
                        new(addr+done) type();
                        ++done;
                    }
                }
                catch(...) { quit(done); throw; }
            }
        };



        template <typename T, typename ALLOCATOR = memory::dyadic> class qmatrix :
        public readable< qvector<T> >,
        public dynamic
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);
            typedef qvector<T>     qrow;
            typedef readable<qrow> rd_t;
            typedef typename rd_t::const_type const_qrow;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit qmatrix(const size_t dims) :
            dimension( constellation::checked_dimension(dims) ),
            situation( constellation::initial_situation(dims) ),
            evaluated(0),
            row(0), idx(0), obj(0), wksp(0), wlen(0)
            {
                init();
            }

            inline virtual ~qmatrix() throw() { quit(); }

            //__________________________________________________________________
            //
            // dynamic interface
            //__________________________________________________________________
            inline virtual  size_t granted() const throw() { return wlen; }

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            inline virtual size_t       size()                        const throw() { return evaluated; }
            inline virtual const_qrow & operator[](const size_t indx) const throw()
            {
                assert(indx>=1); assert(indx<=evaluated);
                return row[indx];
            }

            template <typename U> inline
            bool grow( const readable<U> &user )
            {

                return false;
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t   dimension;
            const maturity situation;
            const size_t   evaluated;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qmatrix);
            qrow   *row; // row[1:dimension]
            size_t *idx; // idx[dimension]
            type   *obj; // obj[dimension^2]
            void   *wksp;
            size_t  wlen;

            inline void quit() throw()
            {
                static memory::allocator &mem = ALLOCATOR::location();
                kill_rows(dimension);
                mem.release(wksp,wlen);
            }

            inline void init()
            {
                allocate();
                make_rows();
            }

            inline void allocate()
            {
                static memory::allocator &mem = ALLOCATOR::instance();
                {
                    memory::embed emb[] =
                    {
                        memory::embed(row,dimension),
                        memory::embed(idx,dimension),
                        memory::embed(obj,dimension*dimension)
                    };
                    wksp = YACK_MEMORY_EMBED(emb,mem,wlen);
                    --row;
                }
            }

            inline void kill_rows(size_t num) throw()
            {
                qrow  *r   = row+1;
                while(num-- > 0) destruct( &r[num] );
            }

            inline void make_rows()
            {
                size_t n   = 0;
                try {
                    qrow  *r   = row+1;
                    type  *p   = obj;
                    while(n<dimension) {
                        new (r+n) qrow(p,dimension);
                        ++n;
                        p += dimension;
                    }
                }
                catch(...) { kill_rows(n); throw; }

            }

        };






    }


}

#endif

