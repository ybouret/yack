

#include "yack/woven/qbranch.hpp"
#include "yack/system/imported.hpp"



namespace yack
{
    namespace woven
    {

        const char qbranch:: clid[] = "qbranch";

        qbranch:: ~qbranch() noexcept
        {
        }

        

        qbranch:: qbranch(const size_t dims, const indices::fund &_)   :
        metrics(dims),
        qfamilies(),
        fund(_)
        {

        }

        size_t qbranch:: check_ndof(const matrix_metrics &M) const
        {
            const size_t dims = M.cols;
            const size_t ndof = M.rows;
            if(dims!=dimensions) throw imported::exception(clid,"invalid matrix #cols/dimensions");
            return ndof;
        }
        
        void qbranch:: raise_null() const
        {
            throw imported::exception(clid,"null vector provided");
        }

        void qbranch:: raise_greater_rank() const
        {
            throw imported::exception(clid,"data matrix has greater rank");
        }

        void qbranch:: raise_smaller_rank() const
        {
            throw imported::exception(clid,"data matrix has smaller rank");
        }

        
        void qbranch:: initialize(const size_t ndof)
        {
            // sanity
            assert(ndof>=1);

            // adjust size
            while(size>ndof) delete pop_back();
            while(size<ndof) (void) push_back( new qfamily(dimensions,fund) );
            assert(ndof==size);

            // initialize families
            size_t i=1;
            for(qfamily *f=head;f;f=f->next,++i)
                f->initialize(ndof,i);

        }

        std::ostream & operator<<(std::ostream &os, const qbranch &self)
        {

            os << '{' << std::endl;
            for(const qfamily *f=self.head;f;f=f->next)
            {
                os << *f << std::endl;
            }
            return os << '}';
        }
        

        void qbranch:: reduce(qfamilies &lhs, qfamilies &rhs) noexcept
        {
            while(rhs.size)
            {
                auto_ptr<qfamily> source = rhs.pop_front();
                bool              keepIt = true;
                for(qfamily *target=lhs.head;target;target=target->next)
                {
                    if(target->used==source->used)
                    {
                        keepIt = false;
                        break;
                    }
                }
                if(keepIt) lhs.push_back( source.yield() );
            }
        }
       


    }

}

