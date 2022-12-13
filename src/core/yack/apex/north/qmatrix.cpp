#include "yack/apex/north/qmatrix.hpp"
#include "yack/randomized/bits.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/memory/embed.hpp"

namespace yack
{
    namespace north
    {

        qmatrix:: qmatrix(const size_t dims) :
        collection(), qmetrics( dims ), readable<qrow>(),
        situation( in_progress ),
        evaluated(0),
        idx(NULL), lib(), obj(), row()
        {
            initialize();
        }

        qmatrix:: qmatrix(const qmatrix &Q) :
        collection(), qmetrics(Q), readable<qrow>(),
        situation(Q.situation),
        evaluated(Q.evaluated),
        idx(NULL), lib(), obj(), row()
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
            const size_t ngs = extra    *dimension;
            qrow        *prw = 0;
            const size_t nrw = dimension;
            apq         *pit = 0;
            const size_t nit = dsq+ngs;

            //--------------------------------------------------------------
            // build top-level shelf
            //--------------------------------------------------------------
            {
                memory::embed emb[] =
                {
                    memory::embed(prw,nrw),
                    memory::embed(idx,dimension),
                    memory::embed(pit,nit)
                };
                lib.build(emb, sizeof(emb)/sizeof(emb[0]),mem);
            }

            //--------------------------------------------------------------
            // build obj
            //--------------------------------------------------------------
            { contractor<apq>  _obj(pit,nit); _obj.swap_with(obj); }

            //--------------------------------------------------------------
            // build rows
            //--------------------------------------------------------------
            { contractor<qrow> _row(prw,nrw,pit+ngs,dimension); _row.swap_with(row); }

            
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
        
        const readable<apq> & qmatrix:: last() const throw() {
            assert(evaluated>0);
            return row[evaluated];
        }
        
        
        bool qmatrix:: complement(writable<apq> &u_k)
        {
            assert(evaluated<dimension);
            const size_t    following = evaluated+1;
            const qrow     &component = row[following];
            thin_array<apq> target( &coerce(component[1]), dimension);
            if(!constellation::prepare_vector(target,u_k,coerce(component.norm2)))
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
                coerce(situation) = updated_situation(dimension,evaluated);
                reschedule();
                return true;
            }
        }
        

    }

}
