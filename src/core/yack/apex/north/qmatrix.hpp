//! \file

#ifndef YACK_APEX_NORTH_QMATRIX_INCLUDED
#define YACK_APEX_NORTH_QMATRIX_INCLUDED 1

#include "yack/apex/north/qvector.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/memory/embed.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/ptr/contractor.hpp"
#include "yack/memory/shelf.hpp"
#include "yack/randomized/bits.hpp"

namespace yack
{

    namespace north
    {

        struct qmatrices;

        //______________________________________________________________________
        //
        //
        //! matrix of orthogonal vector(s)
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR = memory::dyadic>
        class qmatrix : public qmetrics, public readable< qvector<T> >, public dynamic
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
            static  const size_t              extra = 2;      //!< extra arrays

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! default construction based on positive dimension
            inline explicit qmatrix(const size_t dims) :
            collection(), qmetrics( dims ), readable<qrow>(), dynamic(),
            situation( constellation::initial_situation(dims) ),
            evaluated(0),
            idx(NULL), lib(), obj(), row(), qgs()
            {
                allocate();
            }

            //! hard copy
            qmatrix(const qmatrix &Q) :
            collection(), qmetrics(Q), readable<qrow>(), dynamic(),
            situation(Q.situation),
            evaluated(Q.evaluated),
            idx(NULL), lib(), obj(), row(), qgs()
            {
                allocate();
                duplicate(Q);
            }

            //! cleanup
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
            //! Knuth shuffle, mostly to debug
            //__________________________________________________________________
            inline void shuffle(randomized::bits &ran) throw()
            {
                for(size_t i=evaluated;i>1;--i)
                {
                    const size_t j = 1+ran.leq(i-1); assert(j<=i);
                    if(i==j) continue;
                    row[i].xch(row[j]);
                }
                rebuild_index();
            }

            inline const readable<type> & last() const throw() {
                assert(evaluated>0);
                return row[evaluated];
            }

            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const maturity situation; //!< family situation
            const size_t   evaluated; //!< number of members in the family

        private:
            YACK_DISABLE_ASSIGN(qmatrix);
            friend struct qmatrices;

            size_t          *idx; //!< idx[dimension]
            memory::shelf    lib; //!< linear memory
            contractor<type> obj; //!< obj[dimension*dimension]
            contractor<qrow> row; //!< row[dimension]
            contractor<apq>  qgs; //!< qgs[extra*dimension]
            
            inline void rebuild_index() throw()
            {
                thin_array<size_t> qindex(idx,evaluated);
                indexing::make(qindex,comparison::lexicographic<const_qrow,const_qrow>,*this);
            }

            inline void allocate()
            {
                //--------------------------------------------------------------
                // get allocator once
                //--------------------------------------------------------------
                static memory::allocator &mem = ALLOCATOR::instance();

                //--------------------------------------------------------------
                // prepare all memory
                //--------------------------------------------------------------

                qrow         *prw = 0; const size_t  nrw = dimension;
                mutable_type *pit = 0; const size_t  nit = dimension*dimension;
                apq          *pxq = 0; const size_t  nxq = extra*dimension;

                //--------------------------------------------------------------
                // build top-level shelf
                //--------------------------------------------------------------
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

                //--------------------------------------------------------------
                // build obj
                //--------------------------------------------------------------
                { contractor<type> _obj(pit,nit); _obj.swap_with(obj); }

                //--------------------------------------------------------------
                // build rows
                //--------------------------------------------------------------
                { contractor<qrow> _row(prw,nrw,pit,dimension); _row.swap_with(row); }

                //--------------------------------------------------------------
                // build qgs
                //--------------------------------------------------------------
                { contractor<apq> _qgs(pxq,nxq); _qgs.swap_with(qgs); }
                
            }

            inline void duplicate(const qmatrix &Q)
            {
                assert(Q.evaluated==evaluated);
                assert(Q.situation==situation);
                thin_array<size_t>       indx(idx,evaluated);
                const thin_array<size_t> qndx(Q.idx,evaluated);
                for(size_t i=1;i<=evaluated;++i)
                {
                    const qrow   &source = Q.row[i];
                    qrow         &target = row[i];
                    coerce(target.norm2) = source.norm2;
                    for(size_t j=dimension;j>0;--j)
                        coerce(target[j]) = source[j];
                    assert( comparison::equality(source,target) );
                    indx[i] = qndx[i];
                }
                assert( comparison::equality(indx,qndx) );
            }


        };

        //______________________________________________________________________
        //
        //
        //! operations on qmatrices
        //
        //______________________________________________________________________
        struct qmatrices
        {

            //! test equality using indexed vectors
            template <typename T, typename A, typename U, typename B> static inline
            bool equality(const qmatrix<T,A> &lhs,
                          const qmatrix<U,B> &rhs) throw()
            {
                if(lhs.dimension!=rhs.dimension) return false;
                if(lhs.evaluated!=rhs.evaluated) return false;
                const size_t             size = lhs.evaluated;
                const thin_array<size_t> lhsI(lhs.idx,size);
                const thin_array<size_t> rhsI(rhs.idx,size);


                for(size_t i=size;i>0;--i)
                {
                    if( comparison::disparity(lhs[ lhsI[i] ], rhs[ rhsI[i] ] ) ) return false;
                }

                return true;
            }

            //! test equality of last insertion
            template <typename T, typename A, typename U, typename B> static inline
            bool have_same_last(const qmatrix<T,A> &lhs,
                                const qmatrix<U,B> &rhs) throw()
            {
                assert(lhs.dimension==rhs.dimension);
                assert(lhs.evaluated==rhs.evaluated);
                assert(lhs.evaluated>0);
                const size_t size = lhs.evaluated;

#ifndef NDEBUG
                for(size_t i=1;i<size;++i) {
                    assert( comparison::equality(lhs[i],rhs[i]) );
                }
#endif
                return comparison::equality(lhs[size],rhs[size]);

            }
            


        };




    }


}

#endif

