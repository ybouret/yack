

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

        void qbranch:: collect(zrepo &repo) const
        {
            for(const qfamily *f=head;f;f=f->next)
            {
                if(f->tail) repo.ensure(*(f->tail));
            }
        }



        static inline
        void dispatch(indices::set        &db,
                      const indices::list &indx,
                      const qfamily       &target)
        {
            for(const indices::node *node=indx.head;node;node=node->next)
            {
                const size_t ival = **node;
                if(target.used(ival)) continue;
                db += ival;
            }
        }

        //! merge in target is necessary
        static inline
        bool drop_family(qfamily &source, qfamily &target)
        {
            assert(source.size==target.size);
            const bool same_base = source.base == target.base;
            if( same_base || target.contains_all(source) )
            {
                //std::cerr << " (+) source: base=" << *source.base << " | deps=" << *source.deps << " | indx=" << source.indx << std::endl;
                //std::cerr << " (+) target: base=" << *target.base << " | deps=" << *target.deps << " | indx=" << target.indx << std::endl;

                target.base += source.base;
                target.deps += source.deps;
                {
                    indices::set db( target.indx.cache );
                    dispatch(db,target.indx,target);
                    dispatch(db,source.indx,target);
                    db.replace(target.indx);
                }
                //std::cerr << " (*) target: base=" << *target.base << " | deps=" << *target.deps << " | indx=" << target.indx << std::endl;
                //std::cerr << std::endl;

                //if(!same_base) exit(0);

                return true;
            }


            return false;
        }

        void qbranch:: reduce(qfamilies &F)
        {
            //std::cerr << "[[ REDUCTION ]]" << std::endl;
            qfamilies R;
            while(F.size)
            {
                auto_ptr<qfamily> source = F.pop_front();
                bool              keep = true;
                for(qfamily *target = R.head;target;target=target->next)
                {
                    assert(target->size==source->size);
                    if(drop_family(*source, *target))
                    {
                        keep = false;
                        break;
                    }
                }

                if(keep)
                    R.push_back( source.yield() );
            }
            R.swap_with(F);
        }

        void qbranch:: reduce(qfamilies &lhs, qfamilies &rhs)
        {
            while(rhs.size)
            {
                auto_ptr<qfamily> source = rhs.pop_front();
                bool              keep   = true;
                for(qfamily *target = lhs.head;target;target=target->next)
                {
                    assert(target->size==source->size);
                    if(drop_family(*source, *target))
                    {
                        keep = false;
                        break;
                    }
                }

                if(keep)
                    lhs.push_back( source.yield() );
            }
        }


    }

}

