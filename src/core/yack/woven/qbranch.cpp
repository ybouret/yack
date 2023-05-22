

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

        qbranch:: qbranch(const size_t dims) :
        metrics(dims),
        qfamilies(),
        fund( new indices::bank() )
        {
            
        }

        size_t qbranch:: check_dims(const matrix_metrics &M) const
        {
            const size_t dims = M.cols;
            const size_t rmax = M.rows;
            if(dims!=dimensions)        throw imported::exception(clid,"invalid matrix #cols/dimensions");
            if(rmax<1||rmax>dimensions) throw imported::exception(clid,"invalid matrix #rows/max rank");
            return rmax;
        }
        
        void qbranch:: raise_null() const
        {
            throw imported::exception(clid,"null vector provided");
        }


        
        void qbranch:: initialize(const size_t rmax)
        {
            // sanity
            assert(rmax>=1);
            assert(rmax<=dimensions);

            // adjust size
            while(size>rmax)  delete pop_back();
            while(size<rmax) (void) push_back( new qfamily(dimensions,fund) );
            assert(rmax==size);

            // initialize families
            size_t i=1;
            for(qfamily *f=head;f;f=f->next,++i)
                f->initialize(rmax,i);

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

    }

}

