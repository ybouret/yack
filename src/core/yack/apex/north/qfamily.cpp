#include "yack/apex/north/qfamily.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace north
    {
        qfamily:: ~qfamily() throw()
        {
        }

        qfamily:: qfamily(const qfamily &origin) :
        qbase(origin.qbase),
        basis(origin.basis),
        ready(origin.ready),
        width(origin.width),
        next(0),
        prev(0)
        {
        }

        const char qfamily::clid[] = "north::qfamily";

        void qfamily:: throw_invalid_init(const size_t ir)
        {
            throw imported::exception(clid,"invalid initial vbase[%u]",unsigned(ir));
        }

        void qfamily:: assign_all_indices(const readable<size_t> &rindx)
        {
            const size_t ir = rindx[width];
            basis << ir;
            for(size_t i=1;i<width;++i) ready << rindx[i];
            assert(basis.excludes(ready));
            assert(check_width());
        }

        std::ostream & operator<<(std::ostream &os, const qfamily &self)
        {
            os << *(self.qbase)
            << "@" << *(self.basis)
            << "+" << *(self.ready);
            return os;
        }


        static inline
        void family_merge(qfamily       &house,
                          const qfamily &tribe)
        {
            assert(house.check_width());
            house.basis += tribe.basis;
            house.ready += tribe.ready;
            house.ready -= house.basis;
            assert(house.check_width());
        }


        void qfamily:: reduce_freshly_created(list_of<qfamily> &lineage)
        {
            
            std::cerr << "\t--> reduce_freshly_created" << std::endl;
            list_type surrogate;
            while(lineage.size)
            {
                auto_ptr<qfamily> tribe = lineage.pop_front(); assert(tribe->check_width());
                const qmatrix    &lhs   = **tribe;
                bool              enjoy = true;
                for(qfamily *house=surrogate.head;house;house=house->next)
                {
                    const qmatrix &rhs = **house;
                    if(lhs==rhs)
                    {
                        std::cerr << "\t(*) found multiple " << *house << " and " << *tribe << std::endl;
                        enjoy=false;
                        family_merge(*house,*tribe);
                        break;
                    }
                }

                if(enjoy) surrogate.push_back(tribe.yield());

            }
            surrogate.swap_with(lineage);
        }
        
        void qfamily:: fusion_already_reduced(list_of<qfamily> &target,
                                              list_of<qfamily> &source)
        {

            std::cerr << "==> fusion_already_reduced" << std::endl;
            qfamily::list_type kept;
            while( source.size )
            {
                auto_ptr<qfamily> tribe = source.pop_front();
                const qmatrix    &lhs   = **tribe;
                bool              enjoy = true;
                for(qfamily *house=target.head;house;house=house->next)
                {
                    const qmatrix &rhs = **house;
                    if(lhs==rhs)
                    {
                        std::cerr << "\t(*) found multiple " << *house << " and " << *tribe << std::endl;
                        enjoy = false;
                        family_merge(*house,*tribe);
                        break;
                    }
                }

                if(enjoy)
                    kept.push_back( tribe.yield() );
            }

            target.merge_back(kept);

        }


    }
}
