
//! \file

#ifndef YACK_APEX_NORTH_INCLUDED
#define YACK_APEX_NORTH_INCLUDED 1

#include "yack/apex/north/qvector.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/memory/embed.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sort/indexing.hpp"

namespace yack
{

    namespace north
    {

        template <typename T, typename ALLOCATOR = memory::dyadic>
        class qmatrix : public readable< qvector<T> >, public dynamic
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);
            typedef qvector<T>                qrow;
            typedef typename qrow::l2_type    l2_type;
            typedef readable<qrow>            rd_t;
            typedef typename rd_t::const_type const_qrow;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit qmatrix(const size_t dims) :
            dimension( constellation::checked_dimension(dims) ),
            situation( constellation::initial_situation(dims) ),
            evaluated(0),
            row(0), idx(0), obj(0), wksp(0), wlen(0),
            u_k(dimension),
            v_k(dimension)
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
            bool grow(const readable<U> &user)
            {
                assert(user.size()==dimension);

                //--------------------------------------------------------------
                //
                // load user data
                //
                //--------------------------------------------------------------
                for(size_t i=dimension;i>0;--i)
                    u_k[i] = v_k[i] = user[i];

                //--------------------------------------------------------------
                //
                // try to grow
                //
                //--------------------------------------------------------------
                if(qrow::grow(u_k,v_k,*this))
                {
                    //----------------------------------------------------------
                    //
                    // Gram-Schmidt succeeded
                    //
                    //----------------------------------------------------------
                    assert(evaluated<dimension);
                    const size_t  following = evaluated+1;
                    const qrow   &component = row[following];
                    thin_array<T> interface( &coerce(component[1]), dimension);
                    if(!constellation::prepare_vector(interface,u_k,coerce(component.norm2)))
                    {
                        //------------------------------------------------------
                        // nil vector!! shouldn't happen
                        //------------------------------------------------------
                        assert(0==component.norm2);
                        return false;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // update all
                        //------------------------------------------------------
                        coerce(evaluated) = following;
                        coerce(situation) = constellation::updated_situation(dimension,evaluated);
                        rebuild_index();
                        return true;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // in span or nil vector
                    //
                    //----------------------------------------------------------
                    return false;
                }
            }

            inline friend std::ostream & operator<<( std::ostream &os, const qmatrix &self)
            {
                const size_t             dims = self.evaluated;
                os << '{';
                if(dims)
                {
                    os << self[1];
                    for(size_t i=2;i<=dims;++i) os << " | " << self[i];
                    const thin_array<size_t> qidx(self.idx,dims);
                    os << " @<" << qidx << ">";
                }
                os << '}';
                return os;
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
            vector<apq> u_k;
            vector<apq> v_k;

            inline void rebuild_index() throw()
            {
                thin_array<size_t> qindex(idx,evaluated);
                indexing::make(qindex,qrow::compare,*this);
            }


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

