//! \file

#ifndef YACK_APEX_NORTH_QMATRIX_INCLUDED
#define YACK_APEX_NORTH_QMATRIX_INCLUDED 1

#include "yack/apex/north/qvector.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/memory/embed.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/ptr/contractor.hpp"
#include "yack/memory/shelf.hpp"

namespace yack
{

    namespace north
    {
        //______________________________________________________________________
        //
        //
        //! matrix of orthogonal vector(s)
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR = memory::dyadic>
        class qmatrix : public readable< qvector<T> >, public dynamic
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);                           //!< aliases
            typedef qvector<T>                qrow;           //!< alias
            typedef typename qrow::l2_type    l2_type;        //!< alias
            typedef readable<qrow>            rd_t;           //!< alias
            typedef typename rd_t::const_type const_qrow;     //!< alias
            static  const size_t              extra = 2;      //!<  extra arrays

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! default construction based on positive dimensions
            inline explicit qmatrix(const size_t dims) :
            dimension( constellation::checked_dimension(dims) ),
            situation( constellation::initial_situation(dims) ),
            evaluated(0),
            idx(0), lib(), obj(), row(), qgs()
            {
                init();
            }

            inline virtual ~qmatrix() throw() { }

            //__________________________________________________________________
            //
            // dynamic interface
            //__________________________________________________________________
            inline virtual  size_t granted() const throw() { return lib.bytes; } //!< internal buffer length

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________

            //! current size = evaluated = rank of the family
            inline virtual size_t       size()                        const throw() { return evaluated; }

            //! access components
            inline virtual const_qrow & operator[](const size_t indx) const throw()
            {
                assert(indx>=1); assert(indx<=evaluated);
                return row[indx];
            }

            //__________________________________________________________________
            //
            //! univocal Gram-Schmidt algorithm to grom family
            //__________________________________________________________________
            template <typename U> inline
            bool grow(const readable<U> &user)
            {
                assert(user.size()==dimension);

                thin_array<apq> u_k( qgs(),          dimension );
                thin_array<apq> v_k( qgs(dimension), dimension );

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

            //__________________________________________________________________
            //
            //! human friendly display
            //__________________________________________________________________
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
            //! test exact same last vector, assuming all previous are the same
            //__________________________________________________________________
            template<typename U, typename ANOTHER> inline
            bool has_same_last_than(const qmatrix<U,ANOTHER> &user) const
            {
                const qmatrix &self = *this;
                assert(self.dimension==user.dimension);
                assert(self.evaluated==user.evaluated);
                assert(self.evaluated>0);
#ifndef NDEBUG
                for(size_t i=1;i<evaluated;++i) {
                    assert( self[i].eq(user[i]) );
                }
#endif
                return self[evaluated].eq(user[evaluated]);
            }


            //! helper to create indices
            const size_t &first_index() const throw() { return *idx; }

            //__________________________________________________________________
            //
            //! test exact matrix up to a permutation, same dimension/evaluated
            //__________________________________________________________________
            template <typename U,typename ANOTHER> inline
            bool eq(const qmatrix<U,ANOTHER> &user) const
            {
                const qmatrix &self = *this;
                assert(self.dimension==user.dimension);
                assert(self.evaluated==user.evaluated);
                const size_t             n = evaluated;
                const thin_array<size_t> self_indx(self.idx,n);
                const thin_array<size_t> user_indx( &coerce(first_index()),n);

                for(size_t i=n;i>0;--i)
                {
                    if( !self[ self_indx[i] ].eq(user[ user_indx[i] ]) )  return false;
                }

                return true;
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t   dimension; //!< space dimension
            const maturity situation; //!< family situation
            const size_t   evaluated; //!< number of members in the family

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qmatrix);
            size_t          *idx;          //!< idx[dimension]
            memory::shelf    lib;
            contractor<type> obj;
            contractor<qrow> row;
            contractor<apq>  qgs;  //!< qgs[extra*dimension] for G-S algo
            
            inline void rebuild_index() throw()
            {
                thin_array<size_t> qindex(idx,evaluated);
                indexing::make(qindex,qrow::compare,*this);
            }


            
            inline void init()
            {
                static memory::allocator &mem = ALLOCATOR::instance();
                
                qrow         *prw = 0;
                const size_t  nrw = dimension;
                
                mutable_type *pit = 0;
                const size_t  nit = dimension*dimension;
                
                apq          *pxq = 0;
                const size_t  nxq = extra*dimension;
                // build top-level shelf
                {

                    memory::embed emb[] =
                    {
                        memory::embed(prw,nrw),
                        memory::embed(idx,dimension),
                        memory::embed(pit,nit),
                        memory::embed(pxq,nxq)
                    };
                    lib.build(emb, sizeof(emb)/sizeof(emb[0]),mem);
                }
                
                // build obj
                { contractor<type> _obj(pit,nit); _obj.swap_with(obj); }
                
                // build rows
                { contractor<qrow> _row(prw,nrw,pit,dimension); _row.swap_with(row); }
                
                // build qgs
                { contractor<apq> _qgs(pxq,nxq); _qgs.swap_with(qgs); }
                
            }

            

            
            



        };






    }


}

#endif

