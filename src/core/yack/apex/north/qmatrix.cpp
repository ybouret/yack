#include "yack/apex/north/qmatrix.hpp"
#include "yack/randomized/bits.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/memory/embed.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace north
    {

        qmatrix:: qmatrix(const size_t dims) :
        collection(),
        object(),
        qmetrics( dims ),
        readable<qrow>(),
        situation( in_progress ),
        evaluated(0),
        idx(NULL),
        lib(),
        obj(),
        row(),
        vgs()
        {
            initialize();
        }

        qmatrix:: qmatrix(const qmatrix &Q) :
        collection(),
        object(),
        qmetrics(Q),
        readable<qrow>(),
        situation(Q.situation),
        evaluated(Q.evaluated),
        idx(NULL),
        lib(),
        obj(),
        row(),
        vgs()
        {
            initialize();
            build_copy(Q);
        }
        
        qmatrix:: ~qmatrix() throw()
        {
        }

        qmatrix *qmatrix:: clone() const
        {
            return new qmatrix(*this);
        }

        
        size_t qmatrix:: size() const throw() { return evaluated; }

        qmatrix::const_qrow & qmatrix:: operator[](const size_t indx) const throw()
        {
            assert(indx>=1); assert(indx<=evaluated);
            return row[indx];
        }

        void qmatrix:: reschedule() throw()
        {
            thin_array<size_t> qindex(idx,evaluated);
            indexing::make(qindex,comparison::lexicographic<const_qrow,const_qrow>,*this);
        }

        void qmatrix:: shuffle(randomized::bits &ran) throw()
        {
            for(size_t i=evaluated;i>1;--i)
            {
                const size_t j = 1+ran.leq(i-1); assert(j<=i);
                if(i==j) continue;
                row[i].xch(row[j]);
            }
            reschedule();
        }

        void qmatrix:: initialize()
        {
            //--------------------------------------------------------------
            // get allocator once
            //--------------------------------------------------------------
            static memory::allocator &mem = memory::dyadic::instance();

            //--------------------------------------------------------------
            // prepare all memory
            //--------------------------------------------------------------
            const size_t dsq = dimension*dimension;
            qrow        *prw = 0;
            const size_t nrw = dimension;
            apz         *pit = 0;
            const size_t nit = dsq;
            apq         *pgs = 0;
            const size_t ngs = dimension;

            //--------------------------------------------------------------
            // build top-level shelf
            //--------------------------------------------------------------
            {
                memory::embed emb[] =
                {
                    memory::embed(prw,nrw),
                    memory::embed(idx,dimension),
                    memory::embed(pit,nit),
                    memory::embed(pgs,ngs)
                };
                lib.build(emb, sizeof(emb)/sizeof(emb[0]),mem);
            }

            //--------------------------------------------------------------
            // build obj
            //--------------------------------------------------------------
            { contractor<apz>  _obj(pit,nit); _obj.swap_with(obj); }

            //--------------------------------------------------------------
            // build rows
            //--------------------------------------------------------------
            { contractor<qrow> _row(prw,nrw,pit,dimension); _row.swap_with(row); }

            //--------------------------------------------------------------
            // build vgs
            //--------------------------------------------------------------
            { contractor<apq> _vgs(pgs,ngs); _vgs.swap_with(vgs); }

            
        }


        void qmatrix:: build_copy(const qmatrix &Q)
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


        std::ostream & operator<<( std::ostream &os, const qmatrix &self)
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
        
        const readable<apz> & qmatrix:: last() const throw() {
            assert(evaluated>0);
            return row[evaluated];
        }
        


        bool qmatrix:: equality(const qmatrix &lhs,
                                const qmatrix &rhs) throw()
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

        bool operator==(const qmatrix &lhs , const qmatrix &rhs) throw()
        {
            return qmatrix::equality(lhs,rhs);
        }

    }

}
